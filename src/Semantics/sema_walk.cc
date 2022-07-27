#include "AST.h"
#include "Types.h"
#include "Semantics/Analyzer.h"
#include "GC.h"
#include "Error.h"
#include "Debug.h"

namespace Metro::Semantics {
  using ASTKind   = AST::Kind;
  using TypeCon   = TypeContext;

  TypeCon Analyzer::walk(AST::Base* ast) {
    if( !ast ) { // null-pointer
      return { };
    }
    else if( caches.contains(ast) ) { // already walked
      return caches[ast];
    }

    auto& ret = caches[ast];

    ret.ast = ast;
    ret.cond = TypeCon::Condition::Inferred;

    switch( ast->kind ) {
      case ASTKind::Boolean: {
        ret = ValueType::Kind::Bool;
        break;
      }

      case ASTKind::Value: {
        auto x = (AST::Value*)ast;
        auto obj = gcnew();

        switch( ast->token->kind ) {
          case TokenKind::Int:
            obj->type = ValueType::Kind::Int;
            obj->v_int = atoi(ast->token->str.data());
            break;
        }

        x->object = obj;
        ret = obj->type;
        break;
      }

      case ASTKind::Variable: {
        auto& type = walk_lval(ast);

        if( type.cond == TypeCon::Condition::None ) {
          Error::add_error(ErrorKind::UninitializedValue, ast->token, "uninitialized value");
          Error::exit_app();
        }

        break;
      }

      //
      // assign
      //
      case ASTKind::Assign: {
        auto x = (AST::Expr*)ast;
        auto init = walk(x->rhs);

        auto& dest = walk_lval(x->lhs);

        append_assign(dest, x->rhs);
        dest.cond = TypeCon::Condition::Inferred;

        ret = dest;
        break;
      }

      //
      // if
      //
      case ASTKind::If: {
        auto x = (AST::If*)ast;

        auto cond = walk(x->cond);

        if( !cond.equals_kind(ValueType::Kind::Bool) ) {
          Error::add_error(ErrorKind::TypeMismatch, x->cond, "condition must be boolean expression");
        }

        ret = walk(x->if_true);

        if( x->if_false ) {
          auto ctx_false = walk(x->if_false);

          for( auto&& last : ctx_false.canbe_last ) {
            ret.canbe_last.emplace_back(last);
          }
        }

        break;
      }

      //
      // let
      //
      case ASTKind::Let: {
        auto x = (AST::Let*)ast;

        //auto [var_scope, var_typecon] = find_var(x->name);

        auto var_scope = &get_cur_scope();
        auto var_typecon = &var_scope->variable_types[x->name];

        debug(
          alert;
          fprintf(stderr, "%p %p\n", var_scope, var_typecon);
        )

        if( x->type != nullptr ) {
          walk(x->type);
        }

        if( var_typecon != nullptr && var_typecon->cond != TypeCon::Condition::None ) {
          Error::add_error(ErrorKind::MultipleDefinition, x->token, "multiple definition");
          break;
        }

        if( x->init ) {
          alert;
          walk(x->init);

          append_assign(*var_typecon, x->init);
          var_typecon->cond = TypeCon::Condition::Inferred;
        }

        break;
      }

      case ASTKind::Scope: {
        auto x = (AST::Scope*)ast;

        scopelist.push_front(ast);

        auto& scope = get_cur_scope();
        scope.ast = ast;

        for( auto&& elem : x->elems ) {
          walk(elem);
          scope.cur_index++;
        }

        scopelist.pop_front();
        break;
      }

      default: {
        auto x = (AST::Expr*)ast;
        auto lhs = walk(x->lhs);
        auto rhs = walk(x->rhs);


        ret = lhs;
        break;
      }
    }

    return ret;
  }
}