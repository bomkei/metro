//#include <format> // f**

#include "Utils.h"
#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"

namespace Metro::Debug {
  #define strss(x) std::string(x)

  template <class... Args>
  auto format(char const* fmt, Args&&... args) -> std::string {
    static char buf[0x1000];
    sprintf(buf, fmt, args...);
    return buf;
  }

  std::string node2s(Node* node) {
    if( !node ) {
      return "<null node>";
    }

    switch( node->kind ) {
      case ND_VALUE:
        return node->nd_object->to_string();
      
      case ND_VARIABLE:
        return strss(node->token->str);
      
      case ND_CALLFUNC: {
        //auto s = strss(node->token->str);
        
        return strss(node->token->str) + Utils::join<Node*>(", ", node->list, node2s);
      }
    }

    return format("<unknown node: %d>", node->kind);
  }
}