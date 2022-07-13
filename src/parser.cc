#include "metro.h"

Parser::Parser(Token* token)
  : cur(token),
    ate(nullptr)
{
}

Node* Parser::factor() {
  
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

Node* Parser::parse() {
  auto node = new Node(ND_SCOPE, nullptr);

  while( check() ) {
    node->append(func());
  }

  return node;
}

bool Parser::check() {
  return cur->kind != TOK_END;
}

void Parser::next() {
  cur = cur->next;
}

bool Parser::eat(std::string_view const& str) {
  if( cur->str == str ) {
    ate = cur;
    next();
    return true;
  }

  return false;
}

void Parser::expect(std::string_view const& str) {
  if( !eat(str) ) {
    crash;
  }
}

void Parser::expect_ident() {
  if( cur->kind != TOK_IDENT ) {
    crash;
  }
}

Node* Parser::makeexpr(Node* node) {
  if( node->kind == ND_EXPR )
    return node;

  auto x = new Node(ND_EXPR, node->token);
  x->expr_append(EX_BEGIN, node);

  return x;
}

Node* Parser::scope_with_bracket() {
  auto node = new Node(ND_SCOPE, cur);

  expect("{");

  if( eat("}") ) {
    return nullptr;
  }

  while( check() ) {
    node->append(expr());

    if( eat(";") ) {
      if( eat("}") ) {
        node->append(nullptr);
        break;
      }
    }
    else if( eat("}") ) {
      break;
    }
  }

  return node;
}

