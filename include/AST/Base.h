#pragma once

namespace Metro {
  struct Token;
}

namespace Metro::AST {
  struct Base {
    Kind    kind;
    Token*  token;

  protected:
    Base()
      : kind(Kind::None),
        token(nullptr)
    {
    }

    Base(Kind kind, Token* token)
      : kind(kind),
        token(token)
    {
    }

    virtual ~Base() { }
  };
}