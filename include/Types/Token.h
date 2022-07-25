#pragma once

#include <string>

namespace Metro {
  enum class TokenKind {
    Int,
    Float,
    Char,
    String,
    Ident,
    Punctuator,
    End
  };

  struct Token {
    TokenKind kind;
    Token* prev;
    Token* next;
    std::string_view str;
    size_t pos;

    Token(TokenKind kind = TokenKind::Int)
      : kind(kind),
        prev(nullptr),
        next(nullptr),
        pos(0)
    {
    }

    Token(TokenKind kind, Token* prev, size_t pos)
      : Token(kind)
    {
      this->prev = prev;
      this->pos = pos;

      if( prev ) prev->next = this;
    }
  };
}