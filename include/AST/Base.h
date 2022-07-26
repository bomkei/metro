#pragma once

namespace Metro::AST {
  struct Base {
    Kind    kind;
    Token*  token;

    virtual std::pair<size_t, size_t> get_range_on_source() {
      return { token->pos, token->pos + token->str.length() };
    }

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