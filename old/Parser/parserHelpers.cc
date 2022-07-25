#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Parser.h"
#include "Error.h"
#include "Utils.h"

namespace Metro {
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
      Error::add_error(ERR_EXPECTED_TOKEN, cur, "expected '" + std::string(str) + "'");
      Error::exit_app();
    }
  }

  void Parser::expect_ident() {
    if( cur->kind != TOK_IDENT ) {
      Error::add_error(ERR_EXPECTED_TOKEN, cur, "expected identifier");
      Error::exit_app();
    }
  }

  bool Parser::is_need_semi(Node* node) {
    switch( node->kind ) {
      case ND_FUNCTION:
      case ND_SCOPE:
      case ND_FOR:
      case ND_FOREACH:
      case ND_IF:
      case ND_LOOP:
      case ND_WHILE:
        return false;
    }

    return true;
  }

  void Parser::expect_semi() {
    expect(";");
  }

  Node* Parser::expect_type() {
    auto node = new Node(ND_TYPE, cur);

    expect_ident();
    node->nd_name = cur;

    next();

    return node;
  }

  Node* Parser::expect_scope() {
    auto node = new Node(ND_SCOPE, cur);
    auto closed = false;

    expect("{");

    if( eat("}") ) {
      return node;
    }

    while( check() ) {
      auto item = expr();

      node->append(item);

      if( eats(";", "}") ) {
        node->append(nullptr);
        closed = true;
        break;
      }
      else if( eat("}") ) {
        closed = true;
        break;
      }
      else if( is_need_semi(item) ) {
        expect_semi();
      }
    }

    if( !closed ) {
      Error::add_error(ERR_INVALID_SYNTAX, node->token, "not closed scope");
      Error::exit_app();
    }

    return node;
  }

  Node* Parser::makeexpr(Node*& node) {
    

    return nullptr;
  }
}