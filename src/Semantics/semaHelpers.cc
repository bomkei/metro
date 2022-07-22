#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Types/BuiltinFunc.h"
#include "Sema/Analyzer.h"
#include "Utils.h"

namespace Metro::Sema {
  Node* Analyzer::find_func(std::string_view const& name) {
    for( auto&& scope : scope_history ) {
      for( auto&& nd : scope->list ) {
        if( !nd )
          continue;

        switch( nd->kind ) {
          case ND_FUNCTION:
            if( nd->nd_name->str == name )
              return nd;
            
            break;
          
          case ND_BUILTIN_FUNC:
            if( nd->nd_builtin_func->name == name )
              return nd;

            break;
        }
      }
    }

    return nullptr;
  }

  Node* Analyzer::find_var_defined_node(std::string_view const& name) {
    for( auto&& scope : scope_history ) {
      if( scope->kind == ND_SCOPE ) {
        for( auto&& nd : scope->list ) {
          if( nd && nd->kind == ND_LET && nd->nd_name->str == name ) {
            return nd;
          }
        }
      }
      else if( scope->kind == ND_FUNCTION ) {
        for( auto&& arg : scope->list ) {
          if( arg->nd_name->str == name ) {
            return arg;
          }
        }
      }
    }

    return nullptr;
  }

  bool Analyzer::isAddable(TypeInfo left, TypeInfo right) {
    if( !left.equals(right) ) {
      return false;
    }

    switch( left.kind ) {
      case TYPE_INT:
      case TYPE_FLOAT:
        return true;
    }

    return false;
  }
}