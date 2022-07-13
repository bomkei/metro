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
  #define crash     { alert; fprintf(stderr,"\n\t#application has been crashed.\n"); exit(1); }
#else
  #define alert     0
  #define crash     exit(1)
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

  ND_EXPR,
};

enum ExprKind {
  EX_BEGIN,
  EX_ADD,
};

struct Object;
struct Node {
  struct ExprItem {
    ExprKind kind;
    Node*    node;

    explicit ExprItem(ExprKind kind, Node* node)
      : kind(kind),
        node(node)
    {
    }
  };

  NodeKind    kind;
  Token*      token;
  Object*     object;

  Node*  owner;
  std::vector<Node*> nodes;
  std::vector<ExprItem> expr;

  Node(NodeKind kind, Token* token)
    : kind(kind),
      token(token)
  {
  }

  Node* append(Node* node) {
    return nodes.emplace_back(node);
  }

  ExprItem& expr_append(ExprKind kind, Node* node) {
    return expr.emplace_back(kind, node);
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

  bool equals(TypeKind kind) const {
    return this->kind == kind;
  }

  bool equals(TypeInfo const& type) const {
    if( kind != type.kind )
      return false;
    
    return true;
  }

  std::string to_string() const {
    switch( kind ) {
      case TYPE_INT:
        return "int";
      
      case TYPE_NONE:
        return "none";
    }

    return "(unknown type object)";
  }
};

struct Object {
  TypeInfo    type;
  size_t      ref_count;

  union {
    int64_t     v_int;
  };

  Object(TypeInfo type = TYPE_NONE)
    : type(type),
      ref_count(0)
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

class Parser {
public:
  explicit Parser(Token* token);

  Node* factor();
  Node* add();

  Node* parse();

private:
  bool check();
  void next();
  bool eat(std::string_view const& str);
  void expect(std::string_view const& str);
  Node* makeexpr(Node* node);

  Token* cur;
  Token* ate;
};

class Evaluater {
public:
  Evaluater() { }

  TypeInfo eval(Node* node);

private:

  bool isAddable(TypeInfo left, TypeInfo right);


  std::map<Node*, TypeInfo> caches;
};


