#include "metro.h"

static constexpr char const operators[] =
  "(){}[]<>"
  "+-*/%"
  "&^|~"
  ":;,."
  "!?%";

static constexpr char const* long_operators[] = {
  ">>=",
  "<<=",
  ">=",
  "<=",
};

Lexer::Lexer(std::string const& source)
  : source(source),
    position(0),
    length(source.length())
{
}

bool Lexer::check() {
  return position < length;
}

char Lexer::peek() {
  return source[position];
}

bool Lexer::match(std::string_view const& s) {
  return
    position + s.length() <= length &&
    memcmp(source.data() + position, s.data(), s.length()) == 0;
}

void Lexer::pass_space() {
  while( check() && peek() <= ' ' ) {
    position++;
  }
}

Token* Lexer::lex() {
  Token top;
  Token* cur = &top;

  pass_space();

  while( check() ) {
    auto ch = peek();
    auto pos = position;
    auto str = source.data() + pos;

    // digits
    if( isdigit(ch) ) {
      while( isdigit(peek()) ) position++;
    }

    // ident
    else if( isalpha(ch) || ch == '_' ) {
      while( isalnum(ch = peek()) || ch == '_' ) position++;
    }
    
    else {
      cur->kind = TOK_PUNCTUATER;

      for( std::string_view s : long_operators ) {
        if( match(s) ) {
          position += s.length();
          goto found_op;
        }
      }

      for( char c : operators ) {
        if( ch == c ) {
          position++;
          goto found_op;
        }
      }

    found_op:
      pass_space();
      continue;
    }

    cur->str = { str, position - pos };
    pass_space();
  }

  cur = new Token(TOK_END, cur, position);
  return top.next;
}