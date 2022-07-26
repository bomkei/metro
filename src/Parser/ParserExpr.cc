#include "Types.h"
#include "Parser.h"
#include "Error.h"

namespace Metro {
  AST::Base* Parser::factor() {

    switch( cur->kind ) {
      case TokenKind::Int: {
        auto ast = new AST::Value(cur);;

        next();
        return ast;
      }

      case TokenKind::Ident: {
        if( cur->next->str == "(" ) {
          auto ast = new AST::CallFunc(cur);
          ast->name = cur->str;

          next();
          next();

          if( !eat(")") ) {
            do {
              ast->args.emplace_back(expr());
            } while( eat(",") );

            expect(")");
          }

          return ast;
        }

        auto ast = new AST::Variable(cur);
        ast->name = cur->str;

        next();
        return ast;
      }
    }

    Error::add_error(ErrorKind::InvalidSyntax, cur, "invalid syntax");
    Error::exit_app();
  }

  AST::Base* Parser::mul() {
    auto x = factor();

    while( check() ) {
      if( eat("*") ) x = new AST::Expr(AST::Kind::Mul, x, factor());
      else if( eat("/") ) x = new AST::Expr(AST::Kind::Div, x, factor());
      else break;
    }

    return x;
  }

  AST::Base* Parser::add() {
    auto x = mul();

    while( check() ) {
      if( eat("+") ) x = new AST::Expr(AST::Kind::Add, x, mul());
      else if( eat("-") ) x = new AST::Expr(AST::Kind::Sub, x, mul());
      else break;
    }

    return x;
  }

  AST::Base* Parser::compare() {
    using ItemKind = AST::Compare::Item::Kind;

    auto x = add();

    while( check() ) {
      if( eat(">") ) AST::Compare::create(x)->append(ItemKind::BiggerLeft, ate, add());
      else if( eat("<") ) AST::Compare::create(x)->append(ItemKind::BiggerRight, ate, add());
      else break;
    }

    return x;
  }

  AST::Base* Parser::expr() {
    auto ast = stmt();

    if( ast ) {
      return ast;
    }

    return add();
  }
}