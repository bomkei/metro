#include "common.h"
#include "token.h"
#include "object.h"
#include "AST.h"

static std::map<Node*, bool> constexpr_map;

bool is_constexpr_node(Node* node) {
  bool val = false;

  if (!node)
    return false;

  if (constexpr_map.contains(node))
    return constexpr_map[node];

  switch (node->kind) {
    case ND_VALUE:
      val = true;
      break;

    case ND_VARIABLE:
      val = false;
      break;

    case ND_FUNC: {
      val = is_constexpr_node(node->func->code);
      break;
    }
  }

  return constexpr_map[node] = val;
}

bool reduce_constexpr(Node* node) {
  if (!node)
    return false;

  TODO_IMPL;
}