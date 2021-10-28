#pragma once

enum TokenKind
{
  TOK_INT,
  TOK_HEX,
  TOK_CHAR,
  TOK_DOUBLE,
  TOK_STRING,
  TOK_IDENT,
  TOK_RESERVED,
  TOK_EOF
};

struct Token {
  TokenKind kind;
  Token* back;
  Token* next;
  char const* str;
  u32 len;
  u32 pos; // position on source code

  bool str_check(char const* str) const;
  void insert(Token* token);
  std::string getstr() const;
};
