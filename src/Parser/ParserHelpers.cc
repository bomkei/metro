#include "Types.h"
#include "Parser.h"

namespace Metro {
  bool Parser::check() {
    return this->cur->kind != TokenKind::End;
  }

  void Parser::next() {
    this->cur = cur;
  }

  bool Parser::eat(std::string_view str) {

  }

  void Parser::expect(std::string_view str) {

  }

  AST::Scope* Parser::expect_scope() {

  }

  AST::Type* Parser::expect_type() {

  }
}