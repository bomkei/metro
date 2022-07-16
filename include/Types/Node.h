#pragma once

#include <vector>
#include <cstring>

#define   nd_name           uni.tok
#define   nd_object         uni.s0.obj
#define   nd_builtin_func   uni.bifun
#define   nd_is_ref         uni.s0.bval[0]
#define   nd_is_const       uni.s0.bval[1]
#define   nd_type           uni.s0.ndval[0]
#define   nd_item           uni.s0.ndval[0]
#define   nd_if_true        uni.s0.ndval[0]
#define   nd_cond           uni.s0.ndval[0]
#define   nd_ret_type       uni.s0.ndval[0]
#define   nd_expr           uni.s0.ndval[1]
#define   nd_code           uni.s0.ndval[1]
#define   nd_init           uni.s0.ndval[2]
#define   nd_if_false       uni.s0.ndval[2]
#define   nd_inclement      uni.s0.ndval[3]

namespace Metro {
  enum NodeKind {
    ND_TYPE,
    ND_ARGUMENT,
    
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
    ND_BUILTIN_FUNC,
    ND_CLASS,

    ND_NAMESPACE,
  };

  enum ExprKind {
    EX_BEGIN,
    EX_ADD,
    EX_SUB,
    EX_MUL,
    EX_DIV,
  };

  struct Token;
  struct Object;
  struct BuiltinFunc;
  struct Node {
    union __attribute__((packed)) __ndUnion {
      struct __s0_t {
        Object* obj;
        bool    bval[2];
        Node*   ndval[4];
      };

      Token*  tok;
      BuiltinFunc const* bifun;
      __s0_t  s0;
    };

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
    Node*       owner;
    __ndUnion   uni;

    std::vector<Node*> list;
    std::vector<ExprItem> expr;

    Node(NodeKind kind, Token* token)
      : kind(kind),
        token(token),
        owner(nullptr)
    {
      memset(&uni, 0, sizeof(__ndUnion));
    }

    Node* append(Node* node);
    ExprItem& expr_append(ExprKind kind, Node* node);
  };
}
