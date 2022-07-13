#pragma once

class Lexer {
public:
  explicit Lexer(std::string const& source);

  Token* lex();

private:
  bool check();
  char peek();
  bool match(std::string_view const& s);
  void pass_space();

  std::string const& source;
  size_t position;
  size_t const length;
};
