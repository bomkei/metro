//#include <format> // f*

#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Utils.h"

namespace Metro::Debug {
  #define strss(x) std::string(x)

  std::string node2s(Node* node) {
    if( !node ) {
      return "(null node)";
    }

    switch( node->kind ) {
      case ND_VALUE:
        return node->nd_object->to_string();
      
      case ND_VARIABLE:
        return strss(node->token->str);
      
      case ND_CALLFUNC: {
        return strss(node->token->str) + Utils::join<Node*>(", ", node->list, node2s);
      }

      case ND_EXPR: {
        return "(expr)";
      }

      case ND_SCOPE: {

      }

      case ND_FUNCTION: {

      }

      case ND_BUILTIN_FUNC: {
        return format();
      }
    }

    return format("(unknown node kind: %d)", node->kind);
  }
}