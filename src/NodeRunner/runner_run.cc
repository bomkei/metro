#include <iostream>
#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "NodeRunner.h"
#include "GC.h"
#include "Utils.h"

Object* NodeRunner::run(Node* node) {
  if( !node ) {
    alert;
    return Object::none;
  }

  switch( node->kind ) {
    case ND_VALUE: {
      return clone(node->object);
    }

    case ND_VARIABLE: {

      break;
    }

    case ND_CALLFUNC: {
      auto const& name = node->token->str;

      std::vector<Object*> args;

      for( auto&& arg : node->nodes ) {
        args.emplace_back(run(arg));
      }

      if( name == "println" ) {
        for( auto&& obj : args ) {
          std::cout << obj->to_string() << std::endl;
        }

        std::cout << std::endl;
      }

      break;
    }
    
    case ND_EXPR: {
      auto obj = run(node->expr[0].node);

      for( auto it = node->expr.begin() + 1; it != node->expr.end(); it++ ) {
        auto term = run(it->node);

        switch( it->kind ) {
          case EX_ADD: {
            term = objAdd(obj, term);
            break;
          }

          case EX_MUL: {
            term = objMul(obj, term);
            break;
          }
        }
      }

      return obj;
    }
  }

  return Object::none;
}