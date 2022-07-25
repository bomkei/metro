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
          for( auto&& arg : ((AST::Function*)scope.first)->args ) {
            if( arg.name == name ) {
              return &arg;
            }
          }
        }
      }
    }

    return nullptr;
  }
}