#pragma once

namespace Metro::AST {
  struct Type;
  struct Let : Variable {
    Type*   type;

    Let(Token* token)
      : Variable(token)
    {
      this->kind = Kind::Let;
    }
  };
}