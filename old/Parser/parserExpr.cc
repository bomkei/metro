#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Parser.h"
#include "Error.h"
#include "Utils.h"

namespace Metro {
  Node* Parser::factor() {
    
    if( eat("(") ) {
      auto x = expr();
      expect(")");
      return x;
    }

    auto node = new Node(ND_VALUE, cur);

    if( eat("true") || eat("false") ) {
      node->token->kind = TOK_BOOL;
    }

    switch( cur->kind ) {
      case TOK_INT:
      case TOK_FLOAT:
      case TOK_BOOL:
      case TOK_CHAR:
      case TOK_STRING:
        next();
        break;
      
      case TOK_IDENT: {
        alert;

        node->kind = ND_VARIABLE;
        node->nd_name = cur;

        next();

        if( eat("(") ) {
          node->kind = ND_CALLFUNC;

          if( !eat(")") ) {
            do {
              node->list.emplace_back(expr());
            } while( eat(",") );

            alert;
            expect(")");
          }
        }

        break;
      }

      default: {
        Error::add_error(ERR_INVALID_SYNTAX, node->token, "invalid syntax");
        Error::exit_app();
      }
    }

    return node;
  }

  Node* Parser::mul() {
    auto x = factor();

    while( check() ) {
      if( eat("+
    }

    return x;
  }

  Node* Parser::add() {
    auto x = mul();

    while( check() ) {
      if( eat
    }

    return x;
  }

  Node* Parser::compare() {
    auto x = add();

    while( check() ) {
      if( eat("<") ) x = new Node(
    }

    return x;
  }

  Node* Parser::expr() {
    if( eat("let") ) {
      auto node = new Node(ND_LET, ate);

      expect_ident();
      node->nd_name = cur;

      next();

      if( eat(":") ) {
        node->nd_type = expect_type();
      }

      if( eat("=") ) {
        node->nd_expr = expr();
      }

      return node;
    }

    if( eat("if") ) {
      auto node = new Node(ND_IF, ate);

      node->nd_cond = expr();

      node->nd_if_true = expect_scope();

      if( eat("else") ) {
        if( cur->str == "if" ) {
          node->nd_if_false = expr();
        }
        else {
          node->nd_if_false = expect_scope();
        }
      }

      return node;
    }

    return compare();
  }
}