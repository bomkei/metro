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
    Node* expect_type();
    Node* expect_scope();
    Node* makeexpr(Node* node);

    bool is_need_semi(Node* node);
    void expect_semi();

    template <class... Args>
    bool eats(Args&&... args) {
      std::vector<char const*> vec{ args... };
      auto bak = cur;

      for( auto&& s : vec ) {
        if( !eat(s) ) {
          cur = bak;
          return false;
        }
      }

      return true;
    }

    Token* cur;
    Token* ate;
  };
}