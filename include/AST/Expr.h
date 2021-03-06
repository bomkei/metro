#pragma once

namespace Metro::AST {
  struct Expr : Base {
    Base*   lhs;
    Base*   rhs;

    explicit Expr(Kind kind, Base* lhs, Base* rhs, Token* tok)
      : lhs(lhs),
        rhs(rhs)
    {
      this->kind = kind;
      this->token = tok;
    }
  };
}