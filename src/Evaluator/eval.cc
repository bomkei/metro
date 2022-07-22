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
      case ND_TYPE:
      case ND_ARGUMENT:
      case ND_FUNCTION:
      case ND_BUILTIN_FUNC:
        alert;
        break;

      case ND_VALUE:
      case ND_VARIABLE:
      case ND_EXPR:
        alert;
        ret = eval_expr(node);
        break;

      case ND_CALLFUNC: {
        std::vector<Object*> args;
        auto callee = node->nd_callee;

        for( auto&& arg : node->list ) {
          args.emplace_back(eval(arg));
        }
        
        if( callee->kind == ND_BUILTIN_FUNC ) {
          alert;
          return callee->nd_builtin_func->func(args);
        }

        alert;
        auto args_bak = std::move(callee->objects);
        callee->objects = std::move(args_bak);

        for( size_t i = 0; auto&& arg : node->list ) {
          alert;
          callee->objects[i++] = eval(arg);
        }

        auto retval = eval(callee->nd_code);

        callee->objects = std::move(args_bak);

        break;
      }

      case ND_SCOPE: {
        enter_scope(node);

        for( auto&& node : node->list ) {
          eval(node);
        }

        leave_scope();
        break;
      }

      default:
        TODO_IMPL;
    }

    return ret;
  }
}