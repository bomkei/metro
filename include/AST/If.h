#pragma once

namespace Metro::AST {
  struct If : Base {
    Base*   cond;
    Base*   if_true;
    Base*   if_false;

    If(Token* tok)
      : Base(Kind::If, tok),
        cond(nullptr),
        if_true(nullptr),
        if_false(nullptr)
    {
    }
  };
}