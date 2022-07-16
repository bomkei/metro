#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Parser.h"
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
      crash;
    }
  }

  void Parser::expect_ident() {
    if( cur->kind != TOK_IDENT ) {
      crash;
    }
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

        continue;
      }

      expect("}");
      break;
    }

    return node;
  }

  Node* Parser::makeexpr(Node* node) {
    if( node->kind == ND_EXPR )
      return node;

    auto x = new Node(ND_EXPR, node->token);
    x->expr_append(EX_BEGIN, node);

    return x;
  }
}