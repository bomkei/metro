#include "Types.h"
#include "Sema/Analyzer.h"
#include "Error.h"

namespace Metro::Sema {
  bool Analyzer::is_lvalue(AST::Base* ast) {
    switch( ast->kind ) {
      case AST::Kind::Variable:
        return true;
    }

    return false;
  }

  AST::Base* Analyzer::find_var_definition(std::string_view name, AST::Base* cur) {
    for( auto&& ctx : scope_history ) {
      switch( ctx.scope->kind ) {
        case AST::Kind::Scope: {
          for( size_t i = 0; auto&& elem : ((AST::Scope*)ctx.scope)->elems ) {
            if( i++ == ctx.cur_index ) {
              break;
            }

            if( elem && elem->kind == AST::Kind::Let && ((AST::Let*)elem)->name == name ) {
              return elem;
            }
          }

          break;
        }

        case AST::Kind::Function: {
          for( auto&& arg : ((AST::Function*)ctx.scope)->args ) {
            if( arg.name == name ) {
              return &arg;
            }
          }
        }
      }
    }

    return nullptr;
  }

  AST::Function* Analyzer::find_function(std::string_view name) {
    for( auto&& ctx : scope_history ) {
      if( ctx.scope->kind == AST::Kind::Scope ) {
        auto scope = (AST::Scope*)ctx.scope;

        for( auto&& elem : scope->elems ) {
          if( elem && elem->kind == AST::Kind::Function && ((AST::Function*)elem)->name == name ) {
            return (AST::Function*)elem;
          }
        }
      }
    }

    return nullptr;
  }

  Analyzer::ScopeContext& Analyzer::get_cur_scope() {
    return *scope_history.begin();
  }

  Analyzer::ScopeContext& Analyzer::enter_scope(AST::Base* ast) {
    auto& ctx = scope_history.emplace_front(ast);

    return ctx;
  }

  void Analyzer::leave_scope() {
    scope_history.pop_front();
  }
}