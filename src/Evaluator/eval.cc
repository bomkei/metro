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

    switch( node->kind ) {
      case ND_VALUE: {
        return clone(node->nd_object);
      }

      case ND_VARIABLE: {

        break;
      }

      case ND_CALLFUNC: {
        auto const& name = node->token->str;

        std::vector<Object*> args;

        for( auto&& arg : node->list ) {
          args.emplace_back(eval(arg));
        }

        if( name == "println" ) {
          for( auto&& obj : args ) {
            std::cout << obj->to_string();
          }

          std::cout << std::endl;
        }

        break;
      }
      
      case ND_EXPR: {
        auto obj = eval(node->expr[0].node);

        for( auto it = node->expr.begin() + 1; it != node->expr.end(); it++ ) {
          calcObj(it->kind, obj, eval(it->node));
        }

        return obj;
      }

      case ND_FUNCTION:
        break;

      case ND_SCOPE: {
        for( auto&& node : node->list ) {
          eval(node);
        }

        break;
      }

      default:
        TODO_IMPL;
    }

    return Object::none;
  }
}