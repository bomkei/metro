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
    node->name = cur;

    next();
    expect("(");

    if( !eat(")") ) {
      do {
        auto arg = new Node(ND_ARGUMENT, cur);

        expect_ident();
        arg->name = cur;

        next();
        expect(":");

        arg->type = expect_type();
      } while( eat(",") );
      expect(")");
    }

    if( eat("->") ) {
      node->type = expect_type();
    }

    node->code = expect_scope();

    return node;
  }

  crash;
}

}