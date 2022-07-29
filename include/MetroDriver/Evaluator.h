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
    Object** eval_lvalue(AST::Base* ast);

    static Object* create_obj(Token* token);

    static std::vector<BuiltinFunc> const& get_builtin_functions();

  private:
    static std::vector<BuiltinFunc> const builtin_funcs;
  };
}