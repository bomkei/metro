#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Sema/Analyzer.h"
#include "Utils.h"

namespace Metro::Sema {
  Node* Analyzer::find_func(std::string_view const& name) {
    for( auto&& scope : scope_history ) {
      for( auto&& item : scope->list ) {
        if( !item ) {
          continue;
        }

        switch( item->kind ) {
          case ND_FUNCTION: {
            if( item->nd_name->str == name ) {
              return item;
            }

            break;
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