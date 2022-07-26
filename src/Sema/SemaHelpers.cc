#include "Types.h"
#include "Sema/Analyzer.h"
#include "Error.h"

namespace Metro::Sema {
  AST::Base* Analyzer::find_var_definition(std::string_view name, AST::Base* cur) {
    for( auto&& scope : scope_history ) {
      switch( scope.first->kind ) {
        case AST::Kind::Scope: {
          for( size_t i = 0; auto&& elem : ((AST::Scope*)scope.first)->elems ) {
            if( i++ == scope.second ) {
              break;
            }

            if( elem && elem->kind == AST::Kind::Let && ((AST::Let*)elem)->name == name ) {
              return elem;
            }
          }

          break;
        }

        case AST::Kind::Function: {
          alert;
          for( auto&& arg : ((AST::Function*)scope.first)->args ) {
            debug(
              alertios("arg.name = " << arg.name);
            )

            if( arg.name == name ) {
              alert;
              return &arg;
            }
          }
        }
      }
    }

    return nullptr;
  }

  AST::Function* Analyzer::find_function(std::string_view name) {
    for( auto&& pair : scope_history ) {
      if( pair.first->kind == AST::Kind::Scope ) {
        alert;
        auto scope = (AST::Scope*)pair.first;

        for( auto&& elem : scope->elems ) {
          alert;

          if( elem && elem->kind == AST::Kind::Function && ((AST::Function*)elem)->name == name ) {
            alert;
            return (AST::Function*)elem;
          }
        }
      }
    }

    return nullptr;
  }
}