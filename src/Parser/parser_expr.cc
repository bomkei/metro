#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Parser.h"
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
        return node;
      
      case TOK_IDENT: {
        alert;

        node->kind = ND_VARIABLE;

        next();

        if( eat("(") ) {
          node->kind = ND_CALLFUNC;

          if( !eat(")") ) {
            do {
              node->nodes.emplace_back(expr());
            } while( eat(",") );

            alert;
            expect(")");
          }
        }

        break;
      }

      default: {
        crash;
      }
    }

    return node;
  }

  Node* Parser::mul() {
    auto x = factor();

    while( check() ) {
      if( eat("*") || eat("/") ) {
        x = makeexpr(x);
        if( ate->str == "*" ) x->expr_append(EX_MUL, factor());
        else if( ate->str == "/" ) x->expr_append(EX_DIV, factor());
      }
      else {
        break;
      }
    }

    return x;
  }

  Node* Parser::add() {
    auto x = mul();

    while( check() ) {
      if( eat("+") || eat("-") ) {
        x = makeexpr(x);
        if( ate->str == "+" ) x->expr_append(EX_ADD, mul());
        else if( ate->str == "-" ) x->expr_append(EX_SUB, mul());
      }
      else {
        break;
      }
    }

    return x;
  }

  Node* Parser::expr() {
    return add();
  }
}