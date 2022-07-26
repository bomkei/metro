#pragma once

namespace Metro::AST {
  struct Argument : Base {
    std::string_view    name;
    Type*               type;
    Object*             value;

    Argument(Token* tok)
      : Base(Kind::Argument, tok),
        type(nullptr),
        value(nullptr)
    {
    }
  };
}