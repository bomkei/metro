#pragma once

namespace Metro {
  struct Token;
  struct Node;
  class Parser {
  public:
    explicit Parser(Token* token);

    Node* factor();
    Node* add();
    Node* mul();
    Node* expr();

    Node* func();

    Node* parse();

  private:
    bool check();
    void next();
    bool eat(std::string_view const& str);
    void expect(std::string_view const& str);
    void expect_ident();
    void expect_semi();
    Node* expect_type();
    Node* expect_scope();
    Node* makeexpr(Node* node);

    Token* cur;
    Token* ate;
  };
}