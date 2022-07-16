#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Sema/Analyzer.h"
#include "Utils.h"

namespace Metro::Sema {
  TypeInfo Analyzer::Check(Node* node) {
    if( !node ) {
      return { };
    }
    
    if( caches.contains(node) ) {
      return caches[node];
    }

    auto& ret = caches[node];

    switch( node->kind ) {
      case ND_TYPE: {
        auto const& name = node->nd_name->str;

        if( name == "int" ) {
          ret.kind = TYPE_INT;
        }
        else {
          crash;
        }

        break;
      }

      case ND_VALUE:
      case ND_VARIABLE:
      case ND_CALLFUNC:
      case ND_SCOPE:
      case ND_EXPR:
        ret = Expr(node);
        break;

      case ND_ARGUMENT:
        ret = Check(node->nd_type);
        break;

      case ND_FUNCTION: {
        // args
        for( auto&& i : node->list ) {
          Check(i);
        }

        // return type
        ret = Check(node->nd_type);

        // code
        Check(node->nd_code);

        break;
      }

      default:
        crash;
    }

    return ret;
  }
}