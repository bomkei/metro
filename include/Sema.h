#pragma once

#include <map>
#include "Types/TypeInfo.h"

namespace Metro {
  struct Node;
}

namespace Metro::Sema {
  class Analyzer {
  public:
    Analyzer() { }

    TypeInfo Sema(Node* node);
    TypeInfo Expr(Node* node);

  private:

    bool isAddable(TypeInfo left, TypeInfo right);


    std::map<Node*, TypeInfo> caches;
  };
}