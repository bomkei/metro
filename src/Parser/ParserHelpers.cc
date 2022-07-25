#include "Types.h"
#include "Parser.h"
#include "Error.h"

namespace Metro {
  bool Parser::check() {
    return cur->kind != TokenKind::End;
  }

  void Parser::next() {
    cur = cur->next;
  }

  bool Parser::eat(std::string_view str) {
    if( cur->str == str ) {
      ate = cur;
      next();
      return true;
    }

    return false;
  }

  void Parser::expect(std::string_view str) {
    if( !eat(str) ) {
      Error::add_error(ErrorKind::ExpectedToken, cur, "expected '" + std::string(str) + "' before this token");
      Error::exit_app();
    }
  }

  void Parser::expect_ident() {
    if( cur->kind != TokenKind::Ident ) {
      Error::add_error(ErrorKind::ExpectedToken, cur, "expected identifier");
    }
  }

  AST::Scope* Parser::expect_scope() {


    return nullptr;
  }

  AST::Type* Parser::expect_type() {
    expect_ident();

    auto ast = new AST::Type(cur);

    ast->name = cur->str;

    next();

    return ast;
  }
}