#include <iostream>
#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Evaluator.h"
#include "GC.h"
#include "Utils.h"

namespace Metro {
  Object* Evaluator::eval_expr(Node* node) {
    switch( node->kind ) {
      case ND_VALUE: {
        return clone(node->nd_object);
      }

      case ND_VARIABLE: {
        return node->nd_vardef->uni.obj;
      }

      case ND_EXPR: {
        alert;

        auto obj = eval(node->expr[0].node);

        for( auto it = node->expr.begin() + 1; it != node->expr.end(); it++ ) {
          calcObj(it->kind, obj, eval(it->node));
        }

        return obj;
      }
    }

    return Object::none;
  }
}