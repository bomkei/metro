#pragma once

namespace Metro::AST {
  struct Scope : Base {
    std::vector<Base*>  elems;

    Scope(Token* tok)
      : Base(Kind::Scope, tok)
    {
    }
  };
}