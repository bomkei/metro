#pragma once

namespace Metro {
  struct BuiltinFunc;
}

namespace Metro::AST {
  struct Function;
  struct CallFunc : Base {
    std::string_view    name;
    std::vector<Base*>  args;
    Function*           callee;
    BuiltinFunc const*  callee_builtin;

    CallFunc(Token* token)
      : Base(Kind::Callfunc, token),
        callee(nullptr),
        callee_builtin(nullptr)
    {
    }
  };
}