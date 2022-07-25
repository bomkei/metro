#pragma once

#include <list>
#include "Types/Node.h"

namespace Metro {
  struct Object;
  class Evaluator {
  public:
    Evaluator();
    ~Evaluator();

    Object* eval(Node* node);
    Object* eval_expr(Node* node);

    Object* clone(Object* obj);
    Object* calcObj(ExprKind kind, Object* left, Object* right);

  private:

    void enter_scope(Node* scope);
    void leave_scope();

    Node* get_current_scope();

    size_t func_call_count = 0;
    std::list<Node*> scope_history;
  };
}