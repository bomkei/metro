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
      case ND_EXPR:
        alert;
        ret = eval_expr(node);
        break;

      case ND_CALLFUNC: {
        std::vector<Object*> args;
        std::vector<Object*> args_bak;

        auto callee = node->nd_callee;

        for( auto&& arg : node->list ) {
          args.emplace_back(eval(arg));
        }

        if( callee->kind == ND_BUILTIN_FUNC ) {
          auto const& name = callee->nd_builtin_func->name;

          alertios("called builtin func " << COL_BLUE << name);

          ret = callee->nd_builtin_func->func(args);

          alertios("finished builtin func " << COL_BLUE << name);
          break;
        }

        alert;
        //auto args_bak = std::move(callee->objects);
        //callee->objects = std::move(args_bak);

        for( auto it = callee->list.begin(); auto&& obj : args ) {
          (*it++)->uni.obj = obj;
        }

        enter_scope(callee);

        ret = eval(callee->nd_code);

        //callee->objects = std::move(args_bak);

        leave_scope();

        for( size_t i = 0; i < args_bak.size(); i++ ) {
          callee->list[i]->uni.obj = args_bak[i];
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
        TODO_IMPL;
    }

    return ret;
  }
}