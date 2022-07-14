#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Evaluater.h"
#include "Utils.h"

TypeInfo Evaluater::eval(Node* node) {
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
      ret = eval_expr(node);
      break;

    case ND_ARGUMENT:
      ret = eval(node->type);
      break;

    case ND_FUNCTION: {
      // args
      for( auto&& i : node->nodes ) {
        eval(i);
      }

      // return type
      ret = eval(node->type);

      // code
      eval(node->code);

      break;
    }

    default:
      crash;
  }

  return ret;
}