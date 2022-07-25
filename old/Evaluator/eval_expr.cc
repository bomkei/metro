#include <iostream>
#include "Utils.h"
#include "Types.h"
#include "Evaluator.h"
#include "Error.h"
#include "GC.h"

namespace Metro {
  Object* Evaluator::eval_expr(Node* node) {
    Object* ret = Object::none;

    switch( node->kind ) {
      case ND_VALUE: {
        //return clone(node->nd_object);
        return node->nd_object;
      }

      case ND_VARIABLE: {
        return node->nd_vardef->uni.obj;
      }

      case ND_CALLFUNC: {
        std::vector<Object*> args;
        std::vector<Object*> args_bak;

        auto callee = node->nd_callee;

        for( auto&& arg : node->list ) {
          alert;
          args.emplace_back(eval(arg));
        }

        if( callee->kind == ND_BUILTIN_FUNC ) {
          auto const& name = callee->nd_builtin_func->name;

          alertios(COL_GREEN "call builtin func " COL_WHITE << name);

          ret = callee->nd_builtin_func->func(args);

          alertios(COL_GREEN "finished builtin func " COL_WHITE << name);
          break;
        }

        alert;
        //auto args_bak = std::move(callee->objects);
        //callee->objects = std::move(args_bak);

        for( auto it = callee->list.begin(); auto&& obj : args ) {
          (*it++)->uni.obj = obj;
        }

        enter_scope(callee);

        func_call_count++;

        if( func_call_count > 10000 ) {
          Error::add_error(ERR_STACK_OVERFLOW, node->token, "stack overflow");
          Error::exit_app();
        }

        ret = eval(callee->nd_code);

        //callee->objects = std::move(args_bak);

        leave_scope();

        func_call_count--;

        for( size_t i = 0; i < args_bak.size(); i++ ) {
          callee->list[i]->uni.obj = args_bak[i];
        }

        break;
      }
    }

    return ret;
  }
}