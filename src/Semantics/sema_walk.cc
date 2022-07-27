#include "AST.h"
#include "Types.h"
#include "Semantics/Analyzer.h"
#include "Evaluator.h"
#include "GC.h"
#include "Error.h"
#include "Debug.h"
#include "Utils.h"

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
      case ASTKind::Type: {
        auto x = (AST::Type*)ast;

        if( x->name == "int" ) ret = ValueType::Kind::Int;
        else Error::add_error(ErrorKind::UnknownTypeName, x->token, "unknown type name");

        break;
      }

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

          case TokenKind::String:
            obj->type = ValueType::Kind::String;
            obj->v_str = Utils::Strings::to_u16string(std::string(ast->token->str));
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

      case ASTKind::Callfunc: {

        auto x = (AST::CallFunc*)ast;
        auto find = find_func(x->name);

        for( auto&& arg : x->args ) {
          ret.elems.emplace_back(walk(arg));
        }

        if( find == nullptr ) {
          auto const& bifunvec = Evaluator::get_builtin_functions();

          for( auto&& bifun : bifunvec ) {
            if( bifun.name == x->name ) {
              x->callee_builtin = &bifun;
              return bifun.ret_type;
            }
          }

          Error::add_error(ErrorKind::Undefined, x->token, "undefined function name");
          Error::exit_app();
        }

        x->callee = find;
        ret = walk(find->return_type);

        break;
      }

      case ASTKind::Compare: {
        auto x = (AST::Compare*)ast;

        walk(x->first);

        for( auto&& item : x->list ) {
          walk(item.ast);
        }

        ret = ValueType::Kind::Bool;
        break;
      }

      //
      // assign
      //
      case ASTKind::Assign: {
        auto x = (AST::Expr*)ast;
        auto val = must_evaluated(x->rhs);

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
          ret.may_notbe_evaluated = ctx_false.may_notbe_evaluated;

          for( auto&& last : ctx_false.canbe_last ) {
            ret.canbe_last.emplace_back(last);
          }
        }
        else {
          ret.may_notbe_evaluated = true;
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
          must_evaluated(x->init);

          append_assign(*var_typecon, x->init);
          var_typecon->cond = TypeCon::Condition::Inferred;
        }

        break;
      }

      //
      // scope
      //
      case ASTKind::Scope: {
        auto x = (AST::Scope*)ast;

        scopelist.push_front(ast);

        auto& scope = get_cur_scope();
        scope.ast = ast;

        auto it = x->elems.begin();

        for( ; it != x->elems.end() - 1; it++, scope.cur_index++ ) {
          walk(*it);
        }

        ret = walk(*it);

        if( ret.may_notbe_evaluated ) {
          Error::add_error(ErrorKind::MayNotbeEvaluated, ret.ast, "used the value that may not be evaluated for return value");
        }

        scopelist.pop_front();
        break;
      }

      case ASTKind::Function: {
        auto x = (AST::Function*)ast;

        scopelist.push_front(ast);

        auto& scope = get_cur_scope();
        scope.ast = ast;

        for( auto&& arg : x->args ) {
          walk(arg.type);
        }

        ret = walk(x->return_type);

        if( !ret.equals(walk(x->code)) ) {
          Error::add_error(ErrorKind::TypeMismatch, ast->token, "type mismatch");
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