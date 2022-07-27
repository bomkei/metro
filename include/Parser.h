#pragma once

#include "AST.h"

namespace Metro {
  class Parser {
  public:
    explicit Parser(Token* token);

    AST::Base* factor();
    AST::Base* mul();
    AST::Base* add();
    AST::Base* compare();
    AST::Base* assign();
    
    AST::Base* expr();
    AST::Base* stmt();

    AST::Base* func();

    AST::Base* parse();

  private:
    bool check();
    void next();
    bool eat(std::string_view str);

    void expect(std::string_view str);
    void expect_ident();
    AST::Scope* expect_scope();
    AST::Type* expect_type();

    bool is_need_semi(AST::Base* ast);
    void expect_semi();

    Token*  cur;
    Token*  ate;
  };
}