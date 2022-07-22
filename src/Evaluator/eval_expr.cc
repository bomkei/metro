#include <iostream>
#include "Utils.h"
#include "Types.h"
#include "Evaluator.h"
#include "Error.h"
#include "GC.h"

namespace Metro {
  Object* Evaluator::eval_expr(Node* node) {
    switch( node->kind ) {
      case ND_VALUE: {
        return clone(node->nd_object);
      }

      case ND_VARIABLE: {
        return node->nd_vardef->uni.obj;
      }

      case ND_CALLFUNC: {
        std::vector<Object*> args;
        std::vector<Object*> args_bak;
        Object* ret;

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