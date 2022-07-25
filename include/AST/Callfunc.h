#pragma once

namespace Metro::AST {
  struct CallFunc : Base {
    std::string_view    name;
    std::vector<Base*>  args;

    CallFunc(Token* token)
      : Base(Kind::Callfunc, token)
    {
    }
  };
}