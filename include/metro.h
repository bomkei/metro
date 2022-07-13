#pragma once

#include <iostream>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <list>
#include <map>

#define  METRO_DEBUG  1
#define  BIT(n)   (1 << n)

#if METRO_DEBUG
  #define alert     fprintf(stderr,"\t#alert %s:%d\n",__FILE__,__LINE__)
  #define crash     { alert; fprintf(stderr,"\t#application has been crashed.\n"); }
#else
  #define alert     0
  #define crash     0
#endif

enum TokenKind {
  TOK_INT,
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

enum NodeKind {
  ND_VALUE,

  ND_ADD,
};

struct Object;
struct Node {
  NodeKind    kind;
  Token*      token;

  std::vector<Node*> nodes;

  Node(NodeKind kind, Token* token)
    : kind(kind),
      token(token)
  {
  }

  Node(NodeKind kind, Node* lhs, Node* rhs, Token* token)
    : Node(kind, token)
  {
    nodes.emplace_back(lhs);
    nodes.emplace_back(rhs);
  }
};

enum TypeKind {
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_NONE
};

struct TypeInfo {
  TypeKind kind;

  TypeInfo(TypeKind kind = TYPE_NONE)
    : kind(kind)
  {
  }
};

struct Object {
  TypeInfo    type;

  union {
    int64_t     v_int;
  };

  Object(TypeInfo type = TYPE_NONE)
    : type(type)
  {
  }
};

class Lexer {
public:
  explicit Lexer(std::string const& source);

  Token* lex();

private:
  bool check();
  char peek();
  bool match(std::string_view const& s);
  void pass_space();

  std::string const& source;
  size_t position;
  size_t const length;
};

