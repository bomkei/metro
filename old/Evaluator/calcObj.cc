#include "Types/Node.h"
#include "Types/Object.h"
#include "Evaluator.h"
#include "GC.h"
#include "Utils.h"

namespace Metro {
  Object* Evaluator::calcObj(ExprKind kind, Object* left, Object* right) {
    switch( kind ) {
      case EX_ADD:
        switch( left->type.kind ) {
          case TYPE_INT:
            left->v_int += right->v_int;
            break;
        }
        break;

      case EX_SUB:
        switch( left->type.kind ) {
          case TYPE_INT:
            left->v_int -= right->v_int;
            break;
        }
        break;

      case EX_MUL:
        switch( left->type.kind ) {
          case TYPE_INT:
            left->v_int *= right->v_int;
            break;
        }
        break;

      case EX_DIV:
        switch( left->type.kind ) {
          case TYPE_INT:
            left->v_int /= right->v_int;
            break;
        }
        break;

      case EX_BIG_R: {
        switch( left->type.kind ) {
          case TYPE_INT:
            left->v_bool = left->v_int < right->v_int;
            break;
        }
        left->type = TYPE_BOOL;
        break;
      }

      default:
        TODO_IMPL;
    }

    return left;
  }
}