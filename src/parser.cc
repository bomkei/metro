#include "metro.h"

Parser::Parser(Token* token)
  : cur(token),
    ate(nullptr)
{
}

Node* Parser::factor() {
  
  auto node = new Node(ND_VALUE, cur);

  switch( cur->kind ) {
    case TOK_INT:
      next();
      return node;
    
    case TOK_IDENT: {
      node->kind = ND_VARIABLE;
    }

    default: {

      break;
    }
  }

  crash;
}

Node* Parser::add() {
  auto x = factor();

  while( check() ) {
    if( eat("+") || eat("-") ) {
      x = makeexpr(x);
      if( ate->str == "+" ) x->expr_append(EX_ADD, factor());
    }
    else {
      break;
    }
  }

  return x;
}

Node* Parser::parse() {
  return add();
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

Node* Parser::makeexpr(Node* node) {
  if( node->kind == ND_EXPR )
    return node;

  auto x = new Node(ND_EXPR, node->token);
  x->expr_append(EX_BEGIN, node);

  return x;
}
