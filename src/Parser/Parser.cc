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

    switch( this->cur->kind ) {
      case TokenKind::Int: {
        auto ast = new AST::Value(this->cur);;

        this->next();
        return ast;
      }

      case TokenKind::Ident: {
        if( this->cur->next->str == "(" ) {
          auto ast = new AST::CallFunc(this->cur);

          next();
          next();

          if( !this->eat(")") ) {
            do {
              ast->args.emplace_back(this->expr());
            } while( this->eat(",") );

            expect(")");
          }

          return ast;
        }

        auto ast = new AST::Variable(this->cur);

        next();
        return ast;
      }
    }

    Error::add_error(ErrorKind::InvalidSyntax, this->cur, "invalid syntax");
    Error::exit_app();
  }

  AST::Base* Parser::mul() {
    auto x = this->factor();

    while( this->check() ) {
      if( this->eat("*") ) x = new AST::Expr(AST::Kind::Mul, x, this->factor());
      else if( this->eat("/") ) x = new AST::Expr(AST::Kind::Div, x, this->factor());
      else break;
    }

    return x;
  }

  AST::Base* Parser::add() {
    auto x = this->factor();

    while( this->check() ) {
      if( this->eat("+") ) x = new AST::Expr(AST::Kind::Add, x, this->mul());
      else if( this->eat("-") ) x = new AST::Expr(AST::Kind::Sub, x, this->mul());
      else break;
    }

    return x;
  }

  AST::Base* Parser::expr() {
    return this->add();
  }

  AST::Base* Parser::func() {


    return this->expr();
  }

  AST::Base* Parser::parse() {


    return this->func();
  }
}