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

    switch( ast->kind ) {
      case ASTKind::Boolean: {
        ret.type = ValueType::Kind::Bool;
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
        ret.type = obj->type;
        break;
      }

      case ASTKind::Variable: {
        auto x = (AST::Variable*)ast;

        auto [scope, defined] = find_var(x->name);

        if( !scope ) {
          Error::add_error(ErrorKind::Undefined, x->token, "undefined variable name");
          Error::exit_app();
        }

        

        break;
      }

      case ASTKind::Assign: {


        break;
      }

      case ASTKind::If: {
        auto x = (AST::If*)ast;

        auto cond = walk(x->cond);

        if( !cond.type.equals(ValueType::Kind::Bool) ) {
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
        walk(x->init);

        auto scope = get_cur_scope();

        auto& ctx = scope.variable_types[x->name];



        break;
      }

      case ASTKind::Scope: {
        auto x = (AST::Scope*)ast;

        auto& scope = scope_history.emplace_front();
        scope.ast = ast;

        for( auto&& elem : x->elems ) {
          walk(elem);
          scope.cur_index++;
        }

        scope_history.pop_front();
        break;
      }
    }

    return ret;
  }

  void Analyzer::check_symbols() {

  }

  std::pair<ScopeContext*, AST::Base*> Analyzer::find_var(std::string_view name) {
    for( auto&& scopeContext : scope_history ) {
      switch( scopeContext.ast->kind ) {
        case ASTKind::Scope: {
          auto scope = (AST::Scope*)scopeContext.ast;

          for( size_t i = 0; i < scope->elems.size(); i++ ) {
            auto elem = scope->elems[i];

            if( i >= scopeContext.cur_index ) {
              break;
            }

            if( elem && elem->kind == ASTKind::Let && ((AST::Let*)elem)->name == name ) {
              return { &scopeContext, elem };
            }
          }

          break;
        }
      }
    }

    return { };
  }

  ScopeContext& Analyzer::get_cur_scope() {
    return *scope_history.begin();
  }

}