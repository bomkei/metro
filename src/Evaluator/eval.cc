#include <iostream>
#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Evaluator.h"
#include "GC.h"
#include "Utils.h"

namespace Metro {
  Object* Evaluator::eval(Node* node) {
    if( !node ) {
      alert;
      return Object::none;
    }

    auto ret = Object::none;

    switch( node->kind ) {
      case ND_VALUE:
      case ND_VARIABLE:
      case ND_EXPR:
      case ND_SCOPE:
        ret = eval_expr(node);
        break;

      case ND_CALLFUNC:
        ret = eval_callfunc(node);
        break;

      case ND_FUNCTION:
        break;

      default:
        TODO_IMPL;
    }

    return ret;
  }
}