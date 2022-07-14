#include "Types/Node.h"
#include "Types/Object.h"
#include "NodeRunner.h"
#include "GC.h"

Object* NodeRunner::clone(Object* obj) {
  return gcnew(*obj);
}

Object* NodeRunner::objAdd(Object* left, Object* right) {
  
  switch( left->type.kind ) {
    case TYPE_INT:
      left->v_int += right->v_int;
      break;
  }

  return left;
}

Object* NodeRunner::objMul(Object* left, Object* right) {
  
  switch( left->type.kind ) {
    case TYPE_INT:
      left->v_int *= right->v_int;
      break;
  }

  return left;
}
