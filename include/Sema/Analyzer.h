#pragma once

// ----------------------- //
//  Semantics Analyzer
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

    Node* find_func(std::string_view const& name);
    Node* find_var_defined_node(std::string_view const& name);

  private:

    bool isAddable(TypeInfo left, TypeInfo right);

    bool is_branchable(Node* node);
    std::vector<Node*> get_last_nodes(Node* node);

    std::map<Node*, TypeInfo> caches;
    std::list<Node*> scope_history;
  };
}