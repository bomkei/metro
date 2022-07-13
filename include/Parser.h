#pragma once

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
  Node* makeexpr(Node* node);

  Node* scope_with_bracket();

  Token* cur;
  Token* ate;
};
