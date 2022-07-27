#include "AST.h"
#include "Types.h"
#include "Sema/Analyzer.h"
#include "GC.h"
#include "Error.h"

namespace Metro::Sema {
  using ASTKind = AST::Kind;

  TypeContext Analyzer::walk(AST::Base* ast) {

    if( !ast ) {
      return { };
    }
    else if( caches.contains(ast) ) {
      return caches[ast];
    }

    auto& ret = caches[ast];

    ret.ast = ast;
    ret.cond = TypeContext::Condition::Inferred;

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

        if( type.cond == TypeContext::Condition::None ) {
          Error::add_error(ErrorKind::UninitializedValue, ast->token, "uninitialized value");
          Error::exit_app();
        }

        break;
      }

      case ASTKind::Assign: {
        auto x = (AST::Expr*)ast;
        auto init = walk(x->rhs);

        auto& dest = walk_lval(x->lhs);

        append_assign(dest, x->rhs);
        dest.cond = TypeContext::Condition::Inferred;

        ret = dest;
        break;
      }

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

      case ASTKind::Let: {
        auto x = (AST::Let*)ast;

        walk(x->type);

        //walk(x->init);

        auto scope = get_cur_scope();

        auto& ctx = scope.variable_types[x->name];
        auto [scope, typectx] = find_var(x->name);

        if( typectx && typectx->cond != TypeContext::Condition::None ) {
          Error::add_error(ErrorKind::MultipleDefinition, x->token, "multiple definition");
          break;
        }

        ctx.defined = ast;

        if( x->init ) {
          alert;
          walk(x->init);

          append_assign(ctx, x->init);
          ctx.cond = TypeContext::Condition::Inferred;
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

  TypeContext& Analyzer::walk_lval(AST::Base* ast) {
    switch( ast->kind ) {
      case ASTKind::Variable: {
        auto x = (AST::Variable*)ast;
        auto [scope, type] = find_var(x->name);

        if( !scope ) {
          Error::add_error(ErrorKind::Undefined, ast->token, "undefined variable name");
          Error::exit_app();
        }
        else {
          x->defined = type->defined;
        }

        return *type;
      }
    }

    throw 1;
  }

  void Analyzer::check_symbols() {

  }

  std::tuple<ScopeContext*, TypeContext*> Analyzer::find_var(std::string_view name) {
    for( auto&& scope_ast : scopelist ) {
      auto& scopeContext = scopemap[scope_ast];

      switch( scopeContext.ast->kind ) {
        case ASTKind::Scope: {
          auto scope = (AST::Scope*)scopeContext.ast;

          for( size_t i = 0; i < scope->elems.size(); i++ ) {
            auto elem = scope->elems[i];

            if( i >= scopeContext.cur_index ) {
              break;
            }

            if( elem && elem->kind == ASTKind::Let && ((AST::Let*)elem)->name == name ) {
              auto& type = scopeContext.variable_types[name];

              type.defined = elem;

              return { &scopeContext, &type };
            }
          }

          break;
        }
      }
    }

    return { };
  }

  void Analyzer::append_assign(TypeContext& type, AST::Base* ast) {
    auto ctx = walk(ast);

    for( auto&& i : type.assignmented ) {
      if( !walk(i).equals(ctx) ) {
        Error::add_error(ErrorKind::TypeMismatch, ast->token, "type mismatch");
        return;
      }
    }

    type.assignmented.emplace_back(ast);
  }

  ScopeContext& Analyzer::get_cur_scope() {
    return scopemap[*scopelist.begin()];
  }

}