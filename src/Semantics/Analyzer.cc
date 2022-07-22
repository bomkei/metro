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
      case ND_NONE:
        break;

      //
      // Type
      case ND_TYPE: {
        static constexpr std::pair<char const*, TypeKind> typename_list[] {
          { "int", TYPE_INT },
          { "float", TYPE_FLOAT },
          { "bool", TYPE_BOOL },
          { "char", TYPE_CHAR },
          { "string", TYPE_STRING },
          { "args", TYPE_ARGS },
          { "tuple", TYPE_TUPLE },
          { "none", TYPE_NONE }
        };

        auto const& name = node->nd_name->str;

        for( auto&& pair : typename_list ) {
          if( name == pair.first ) {
            return ret = pair.second;
          }
        }

        Error::add_error(ERR_UNKNOWN_TYPE, node->token, "unknown type name");
        Error::exit_app();

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

      //
      // Let-statement
      case ND_LET: {
        auto const is_type_declared = node->nd_type != nullptr;

        if( node->nd_expr ) {
          ret = check(node->nd_expr);

          if( is_type_declared && !ret.equals(check(node->nd_type)) ) {
            Error::add_error(ERR_TYPE_MISMATCH, node->nd_type, "type mismatch");
          }
        }
        else if( is_type_declared ) {
          ret = check(node->nd_type);
        }
        else {
          Error::add_error(ERR_CANNOT_REFER, node, "cannot refer the type of variable '" + std::string(node->nd_name->str) + "'");
          Error::exit_app();
        }

        return TYPE_NONE;
      }

      case ND_FUNCTION: {
        // args
        for( auto&& i : node->list ) {
          check(i);
        }

        // return type
        // ret = check(node->nd_type);

        scope_history.push_front(node);

        // code
        auto code_type = check(node->nd_code);

        if( node->nd_ret_type ) {
          ret = check(node->nd_ret_type);

          if( !ret.equals(code_type) ) {
            Error::add_error(ERR_TYPE_MISMATCH, node->token, "return type");
            Error::exit_app();
          }
        }
        else {
          ret = code_type;
        }

        scope_history.pop_front();
        //node->objects.resize(node->list.size());
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