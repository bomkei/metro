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
        auto const& name = node->name->str;

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
      case ND_EXPR:
      case ND_SCOPE:
        alert;
        ret = Check(node);
        break;

      case ND_ARGUMENT:
        ret = Check(node->type);
        break;

      case ND_FUNCTION: {
        // args
        for( auto&& i : node->nodes ) {
          Check(i);
        }

        // return type
        ret = Check(node->type);

        // code
        Check(node->code);

        break;
      }

      default:
        crash;
    }

    return ret;
  }
}