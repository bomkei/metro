#include "Types.h"
#include "Parser.h"
#include "Error.h"

namespace Metro {
  Parser::Parser(Token* token)
    : cur(token),
      ate(nullptr)
  {
  }

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
    auto x = factor();

    while( check() ) {
      if( eat("+") ) x = new AST::Expr(AST::Kind::Add, x, mul());
      else if( eat("-") ) x = new AST::Expr(AST::Kind::Sub, x, mul());
      else break;
    }

    return x;
  }

  AST::Base* Parser::expr() {
    return add();
  }

  AST::Base* Parser::func() {
    if( eat("fn") ) {
      auto ast = new AST::Function(cur);

      expect_ident();
      ast->name = cur->str;

      next();
      expect("(");

      if( !eat(")") ) {
        do {
          expect_ident();
          auto name = cur->str;

          next();
          expect(":");

          auto type = expect_type();

          ast->args.emplace_back(name, type);
        } while( eat(",") );

        expect(")");
      }
    }

    return expr();
  }

  AST::Base* Parser::parse() {


    return func();
  }
}