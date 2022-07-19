#pragma once

// ----------------------- //
//  Semantics
// ----------------------- //

#include <map>
#include <list>
#include "Types/TypeInfo.h"

namespace Metro {
  struct Node;
}

namespace Metro::Sema {
  class Analyzer {
  public:
    Analyzer() { }

    TypeInfo check(Node* node);
    TypeInfo expr(Node* node);

    Node* find_func_in_scope(Node* scope, std::string_view const& name);
    Node* find_func(std::string_view const& name);

  private:

    bool isAddable(TypeInfo left, TypeInfo right);

    std::map<Node*, TypeInfo> caches;
    std::list<Node*> scope_history;
  };
}