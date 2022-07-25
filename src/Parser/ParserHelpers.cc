#include "Types.h"
#include "Parser.h"
#include "Error.h"

namespace Metro {
  bool Parser::check() {
    return this->cur->kind != TokenKind::End;
  }

  void Parser::next() {
    this->cur = cur;
  }

  bool Parser::eat(std::string_view str) {
    if( this->cur->str == str ) {
      this->ate = cur;
      this->next();
      return true;
    }

    return false;
  }

  void Parser::expect(std::string_view str) {
    if( !this->eat(str) ) {
      Error::add_error(ErrorKind::ExpectedToken, this->cur, "expected '" + std::string(str) + "' before this token");
      Error::exit_app();
    }
  }

  AST::Scope* Parser::expect_scope() {


    return nullptr;
  }

  AST::Type* Parser::expect_type() {


    return nullptr;
  }
}