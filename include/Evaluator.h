#pragma once

#include "Types/Node.h"

namespace Metro {
  struct Object;
  class Evaluator {
  public:
    Evaluator();
    ~Evaluator();

    Object* clone(Object* obj);

    Object* eval(Node* node);

    Object* calcObj(ExprKind kind, Object* left, Object* right);

  private:
  };
}