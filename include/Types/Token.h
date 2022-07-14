#pragma once

#include <string>

namespace Metro {
  enum TokenKind {
    TOK_INT,
    TOK_FLOAT,
    TOK_BOOL,
    TOK_CHAR,
    TOK_STRING,
    TOK_IDENT,
    TOP_OPERATOR,
    TOK_PUNCTUATER,
    TOK_END
  };

  struct Token {
    TokenKind kind;
    Token* prev;
    Token* next;
    std::string_view str;
    size_t pos;

    Token(TokenKind kind = TOK_INT)
      : kind(kind),
        prev(nullptr),
        next(nullptr),
        pos(0)
    {
    }

    Token(TokenKind kind, Token* prev, size_t pos)
      : kind(kind),
        prev(prev),
        next(nullptr),
        pos(pos)
    {
      if( prev ) prev->next = this;
    }
  };
}