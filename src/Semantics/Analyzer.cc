#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Sema/Analyzer.h"
#include "Error.h"
#include "Utils.h"

namespace Metro::Sema {
  //
  // Analyze a node
  TypeInfo Analyzer::check(Node* node) {
    if( !node ) {
      return { };
    }

    if( caches.contains(node) ) {
      return caches[node];
    }

    auto& ret = caches[node];

    switch( node->kind ) {
      //
      // Type
      case ND_TYPE: {
        auto const& name = node->nd_name->str;

        if( name == "int" ) {
          ret = TYPE_INT;
        }
        else {
          Error::add_error(ERR_UNKNOWN_TYPE, node->token, "unknown type name");
          Error::exit_app();
        }

        break;
      }

      //
      // Argument
      case ND_ARGUMENT:
        ret = check(node->nd_type);
        break;

      case ND_VALUE:
      case ND_VARIABLE:
      case ND_CALLFUNC:
      case ND_SCOPE:
      case ND_EXPR:
        ret = expr(node);
        break;

      case ND_LET: {
        if( !node->nd_type && !node->nd_expr ) {
          Error::add_error(ERR_CANNOT_REFER
        }
      }

      case ND_FUNCTION: {
        // args
        for( auto&& i : node->list ) {
          check(i);
        }

        // return type
        ret = check(node->nd_type);

        // code
        check(node->nd_code);

        node->objects.resize(node->list.size());
        break;
      }

      case ND_BUILTIN_FUNC:
        break;

      case ND_NAMESPACE: {
        for( auto&& x : node->list ) {
          check(x);
        }

        break;
      }

      default:
        crash;
    }

    return ret;
  }
}