#pragma once

class Evaluater {
public:
  Evaluater() { }

  TypeInfo eval(Node* node);

private:

  bool isAddable(TypeInfo left, TypeInfo right);


  std::map<Node*, TypeInfo> caches;
};
