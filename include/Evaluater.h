#pragma once

#include <map>
#include "Types/TypeInfo.h"

struct Node;
class Evaluater {
public:
  Evaluater() { }

  TypeInfo eval(Node* node);

  TypeInfo eval_expr(Node* node);

private:

  bool isAddable(TypeInfo left, TypeInfo right);


  std::map<Node*, TypeInfo> caches;
};
