#pragma once

namespace Metro {
  struct Object;
  namespace AST {
    struct Base;
  }

  class Evaluator {
  public:
    Evaluator() { }

    Object* eval(AST::Base* ast);

  };
}