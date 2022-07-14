#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Evaluater.h"
#include "Utils.h"

TypeInfo Evaluater::eval(Node* node) {
  if( caches.contains(node) ) {
    return caches[node];
  }

  auto& ret = caches[node];

  switch( node->kind ) {
    case ND_VALUE:
    case ND_VARIABLE:
    case ND_CALLFUNC:
    case ND_EXPR:
      return eval_expr(node);

    case ND_FUNCTION: {

      break;
    }

    default:
      crash;
  }

  return ret;
}