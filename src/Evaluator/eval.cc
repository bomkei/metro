#include <iostream>
#include "Utils.h"
#include "Debug.h"
#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Types/BuiltinFunc.h"
#include "Evaluator.h"
#include "GC.h"

namespace Metro {
  Object* Evaluator::eval(Node* node) {
    if( !node ) {
      alert;
      return Object::none;
    }

    auto ret = Object::none;

    switch( node->kind ) {
      case ND_NONE:
      case ND_TYPE:
      case ND_ARGUMENT:
      case ND_FUNCTION:
      case ND_BUILTIN_FUNC:
        alert;
        break;

      case ND_VALUE:
      case ND_VARIABLE:
      case ND_CALLFUNC:
      case ND_EXPR:
        alert;
        ret = eval_expr(node);
        break;

      case ND_IF: {
        auto cond = eval(node->nd_cond);

        if( cond->v_bool ) {
          return eval(node->nd_if_true);
        }
        
        return eval(node->nd_if_false);
      }

      case ND_LET: {
        if( node->nd_expr ) {
          node->uni.obj = eval(node->nd_expr);
        }

        break;
      }

      case ND_SCOPE: {
        enter_scope(node);

        // for( auto&& node : node->list ) {
        //   eval(node);
        // }

        for( size_t i = 0; i < node->list.size() - 1; i++ ) {
          eval(node->list[i]);
        }

        ret = eval(*node->list.rbegin());

        leave_scope();
        break;
      }

      default:
        alertios("eval: not implemented node kind: " << node->kind);
        crash;
    }

    return ret;
  }
}