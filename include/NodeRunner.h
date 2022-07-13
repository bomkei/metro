#pragma once

struct Object;
class NodeRunner {
public:
  NodeRunner();
  ~NodeRunner();

  Object* clone(Object* obj);

  Object* run(Node* node);

  Object* objAdd(Object* left, Object* right);
  Object* objMul(Object* left, Object* right);

private:
};
