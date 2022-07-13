#pragma once

enum NodeKind {
  ND_TYPE,
  
  ND_VALUE,
  ND_VARIABLE,
  ND_CALLFUNC,

  ND_ADDR,

  ND_LET,
  ND_IF,
  ND_FOR,
  ND_FOREACH,
  ND_LOOP,
  ND_WHILE,
  ND_DO_WHILE,
  ND_SCOPE,
  ND_EXPR,

  ND_FUNCTION,

  ND_CLASS,
};

enum ExprKind {
  EX_BEGIN,
  EX_ADD,
  EX_SUB,
  EX_MUL,
  EX_DIV,
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

  Node*   code;
  Token*  name;

  Node(NodeKind kind, Token* token)
    : kind(kind),
      token(token),
      object(nullptr),
      owner(nullptr),
      code(nullptr)
  {
  }

  Node* append(Node* node) {
    return nodes.emplace_back(node);
  }

  ExprItem& expr_append(ExprKind kind, Node* node) {
    return expr.emplace_back(kind, node);
  }
};
