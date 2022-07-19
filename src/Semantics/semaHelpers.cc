#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Types/BuiltinFunc.h"
#include "Sema/Analyzer.h"
#include "Utils.h"

namespace Metro::Sema {
  Node* Analyzer::find_func_in_scope(Node* scope, std::string_view const& name) {
    for( auto&& nd : scope->list ) {
      switch( nd->kind ) {
        case ND_FUNCTION:
          if( nd->nd_name->str == name )
            return nd;
          
          break;
        
        case ND_BUILTIN_FUNC:
          if( nd->nd_builtin_func->name == name )
            return nd;

          break;

        case ND_SCOPE: {
          auto find = find_func_in_scope(nd, name);

          if( find )
            return find;
          
          break;
        }
      }
    }

    return nullptr;
  }

  Node* Analyzer::find_func(std::string_view const& name) {
    for( auto&& scope : scope_history ) {
      auto find = find_func_in_scope(scope, name);

      if( find )
        return find;
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