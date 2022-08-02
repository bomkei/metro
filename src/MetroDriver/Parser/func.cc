#include "Types.h"
#include "MetroDriver/Parser.h"
#include "Error.h"

namespace Metro {
  AST::Base* Parser::func() {
    if( eat("fn") ) {
      auto ast = new AST::Function(cur);

      expect_ident();
      ast->name = cur->str;

      next();
      expect("(");

      if( !eat(")") ) {
        do {
          auto& arg = ast->args.emplace_back(cur);

          expect_ident();
          arg.name = cur->str;
          arg.token = cur;

          next();
          expect(":");

          arg.type = expect_type();
        } while( eat(",") );

        expect(")");
      }

      if( eat("->") ) {
        ast->return_type = expect_type();
      }

      ast->code = expect_scope();

      return ast;
    }

    return expr();
  }
}