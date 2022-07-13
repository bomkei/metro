#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Parser.h"
#include "Utils.h"

Node* Parser::func() {
  if( eat("fn") ) {
    auto node = new Node(ND_FUNCTION, ate);

    expect_ident();
    node->name = cur;

    next();
    expect("(");

    if( !eat(")") ) {
      do {

      } while( eat(",") );
      expect(")");
    }

    if( eat("->") ) {

    }
  }

  crash;
}
