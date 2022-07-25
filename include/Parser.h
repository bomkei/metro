#pragma once

#include "AST.h"

namespace Metro {
  class Parser {
  public:
    explicit Parser(Token* token);

    AST::Base* factor();
    AST::Base* mul();
    AST::Base* add();
    AST::Base* expr();

    AST::Base* func();

    AST::Base* parse();

  private:
    bool check();
    void next();
    bool eat(std::string_view str);
    void expect(std::string_view str);
    AST::Scope* expect_scope();
    AST::Type* expect_type();

    Token*  cur;
    Token*  ate;
  };
}