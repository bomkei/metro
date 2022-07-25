#pragma once

namespace Metro::AST {
  struct Argument : Variable {
    Type*     type;
    Object*   value;

    Argument(Token* tok)
      : Variable(tok),
        type(nullptr),
        value(nullptr)
    {
    }
  };
}