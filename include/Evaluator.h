#pragma once

namespace Metro {
  struct Object;
  struct Node;
  class Evaluator {
  public:
    Evaluator();
    ~Evaluator();

    Object* clone(Object* obj);

    Object* eval(Node* node);

    Object* objAdd(Object* left, Object* right);
    Object* objMul(Object* left, Object* right);

  private:
  };
}