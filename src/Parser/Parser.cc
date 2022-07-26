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

  AST::Base* Parser::expr() {
    if( eat("if") ) {
      auto ast = new AST::If(ate);

      ast->cond = expr();
      ast->if_true = expect_scope();

      if( eat("else") ) {
        if( cur->str == "if" ) {
          ast->if_false = expr();
        }
        else {
          ast->if_false = expect_scope();
        }
      }

      return ast;
    }

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
          auto& arg = ast->args.emplace_back(cur);

          expect_ident();
          arg.name = cur->str;

          next();
          expect(":");

          arg.type = expect_type();
        } while( eat(",") );

        expect(")");
      }

      if( eat("->") ) {
        ast->return_type = expect_type();
      }

      ast->code = expect_scope();

      return ast;
    }

    return expr();
  }

  AST::Base* Parser::parse() {
    auto scope = new AST::Scope(nullptr);

    while( check() ) {
      auto x = scope->append(func());

      if( is_need_semi(x) && cur->kind != TokenKind::End ) {
        expect_semi();
      }
    }

    return scope;
  }
}