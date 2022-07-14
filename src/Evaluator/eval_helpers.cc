#include "Types/Node.h"
#include "Types/Object.h"
#include "Evaluator.h"
#include "GC.h"

namespace Metro {
  Object* Evaluator::clone(Object* obj) {
    return gcnew(*obj);
  }

  Object* Evaluator::objAdd(Object* left, Object* right) {
    
    switch( left->type.kind ) {
      case TYPE_INT:
        left->v_int += right->v_int;
        break;
    }

    return left;
  }

  Object* Evaluator::objMul(Object* left, Object* right) {
    
    switch( left->type.kind ) {
      case TYPE_INT:
        left->v_int *= right->v_int;
        break;
    }

    return left;
  }
}