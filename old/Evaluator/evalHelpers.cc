#include "Types/Node.h"
#include "Types/Object.h"
#include "Evaluator.h"
#include "GC.h"
#include "Utils.h"

namespace Metro {
  Object* Evaluator::clone(Object* obj) {
    return gcnew(*obj);
  }

  void Evaluator::enter_scope(Node* scope) {
    scope_history.push_front(scope);
  }

  void Evaluator::leave_scope() {
    scope_history.pop_front();
  }

  Node* Evaluator::get_current_scope() {
    return *scope_history.begin();
  }
}