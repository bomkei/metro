#pragma once

namespace Metro {
  struct Object;
}

namespace Metro::AST {
  struct Value : Base {
    Object*   object;

    Value(Token* tok)
      : Base(Kind::Value, tok)
    {
    }
  };
}