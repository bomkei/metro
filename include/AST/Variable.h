#pragma once

namespace Metro::AST {
  struct Variable : Base {
    std::string_view  name;
    Base*             defined;

    Variable(Token* tok)
      : Base(Kind::Variable, tok),
        defined(nullptr)
    {
    }
  };
}