#include "Types.h"
#include "MetroDriver/Parser.h"
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
    auto ast = new AST::Scope(cur);

    expect("{");

    if( eat("}") ) {
      return ast;
    }

    while( check() ) {
      auto item = expr();

      ast->append(item);

      if( eat(";") ) {
        if( eat("}") ) {
          ast->append(AST::None::val);
          break;
        }
      }
      else if( !eat("}") ) {
        if( is_need_semi(item) ) {
          expect_semi();
        }
      }
      else {
        break;
      }
    }

    return ast;
  }

  AST::Type* Parser::expect_type() {
    expect_ident();

    auto ast = new AST::Type(cur);

    ast->name = cur->str;

    next();

    return ast;
  }

  bool Parser::is_need_semi(AST::Base* ast) {
    using AST::Kind;

    switch( ast->kind ) {
      case Kind::If:
      case Kind::Function:
        return false;
    }

    return true;
  }

  void Parser::expect_semi() {
    expect(";");
  }
}