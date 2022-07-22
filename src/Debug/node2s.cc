//#include <format> // f*

#include "Types.h"
#include "Utils.h"

namespace Metro::Debug {
  #define strss(x) std::string(x)

  std::string node2s(Node* node) {
    if( !node ) {
      return "(null node)";
    }

    switch( node->kind ) {
      case ND_VALUE:
        if( node->nd_object == nullptr ) {
          return "(ND_VALUE: not initialized yet)";
        }

        return node->nd_object->to_string();

      case ND_VARIABLE:
        return strss(node->token->str);

      case ND_CALLFUNC: {
        return strss(node->token->str) + "(" + Utils::join<Node*>(", ", node->list, node2s) + ")";
      }

      case ND_EXPR: {
        return "(expr)";
      }

      case ND_SCOPE: {
        return "{\n" + Utils::join<Node*>("\n", node->list, node2s) + "\n}";
      }

      case ND_FUNCTION: {
        
        

        break;
      }

      case ND_BUILTIN_FUNC: {
        auto bfun = node->nd_builtin_func;
        alert;

        return inlineSS(
            "fn (builtin) " << bfun->name << "("
            << Utils::join<TypeInfo>(", ", bfun->arg_types, [](auto&x){return x.to_string();})
          );
      }
    }

    return Utils::format("(unknown node kind: %d)", node->kind);
  }
}