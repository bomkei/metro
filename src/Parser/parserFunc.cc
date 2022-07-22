#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Parser.h"
#include "Utils.h"

namespace Metro {
  Node* Parser::func() {
    if( eat("fn") ) {
      auto node = new Node(ND_FUNCTION, ate);

      expect_ident();
      node->nd_name = cur;

      next();
      expect("(");

      if( !eat(")") ) {
        do {
          auto arg = new Node(ND_ARGUMENT, cur);

          expect_ident();
          arg->nd_name = cur;

          next();
          expect(":");

          arg->nd_type = expect_type();
          node->append(arg);
        } while( eat(",") );
        expect(")");
      }

      if( eat("->") ) {
        node->nd_ret_type = expect_type();
      }

      node->nd_code = expect_scope();

      return node;
    }

    return expr();
  }
}