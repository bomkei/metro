#pragma once

namespace Metro {
  struct Token;
  struct Node;
  
  struct ErrorContext {
    Token*  token;
    Node*   node;

    size_t  view_begin;
    size_t  view_end;

    size_t  err_begin;
    size_t  err_end;

    std::string_view  src_view;

    void show();
  };

  namespace Error {
    void add_error(Token* token, std::string const& msg);
    void add_error(Node* node, std::string const& msg);

    void show_all();

    void exit_app();
  }
}