#include "Types.h"
#include "Parser.h"
#include "Error.h"

namespace Metro {
  AST::Base* Parser::stmt() {
    if( eat("if") ) {
      auto ast = new AST::If(ate);

      ast->cond = expr();
      ast->if_true = expect_scope();

      if( eat("else") ) {
        if( cur->str == "if" ) {
          ast->if_false = expr();
        }
        else {
          ast->if_false = expect_scope();
        }
      }

      return ast;
    }

    if( eat("let") ) {
      auto ast = new AST::Let(ate);

      expect_ident();
      ast->name = cur->str;

      next();

      if( eat(":") ) {
        ast->type = expect_type();
      }

      if( eat("=") ) {
        ast->init = expr();
      }

      return ast;
    }

    return nullptr;
  }
}