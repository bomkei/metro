#include "Types/Node.h"

namespace Metro {
  Node* Node::append(Node* node) {
    return nodes.emplace_back(node);
  }

  Node::ExprItem& Node::expr_append(ExprKind kind, Node* node) {
    return expr.emplace_back(kind, node);
  }
}