#include "Types.h"
#include "Evaluator.h"

namespace Metro {
  Object* Evaluator::eval(AST::Base* ast) {
    if( !ast ) {
      return Object::none;
    }

    Object* ret = Object::none;

    switch( ast->kind ) {
      case AST::Kind::Value: {
        return ((AST::Value*)ast)->object;
      }
    }

    return ret;
  }
}