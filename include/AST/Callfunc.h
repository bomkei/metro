#pragma once

namespace Metro::AST {
  struct Function;
  struct CallFunc : Base {
    std::string_view    name;
    std::vector<Base*>  args;
    Function*           callee;

    CallFunc(Token* token)
      : Base(Kind::Callfunc, token),
        callee(nullptr)
    {
    }
  };
}