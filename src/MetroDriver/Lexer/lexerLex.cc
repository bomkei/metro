#include <cstring>
#include "Types/Token.h"
#include "Lexer.h"
#include "Error.h"
#include "Utils.h"

static constexpr char const punctuaters[] =
  "(){}[]<>"
  "+-*/%="
  "&^|~"
  ":;,."
  "!?%";

static constexpr char const* long_punctuaters[] = {
  ">>=",
  "<<=",
  "->",
  ">=",
  "<=",
  "+=",
  "-=",
  "*=",
  "/=",
  "%=",
  "++",
  "--",
  "<<",
  ">>",
  "&&",
  "||",
};

namespace Metro {
  Token* Lexer::lex() {
    Token top;
    Token* cur = &top;

    pass_space();

    while( check() ) {
      auto ch = peek();
      auto pos = position;
      auto str = source.data() + pos;

      cur = new Token(TokenKind::Int, cur, pos);

      // digits
      if( isdigit(ch) ) {
        while( isdigit(peek()) ) position++;

        if( peek() == '.' ) {
          position++;

          if( peek() <= ' ' ) {
            cur->kind = TokenKind::Float;
          }
          else if( isalpha(ch == peek()) || ch == '_' ) {
            position--;
          }
        }
      }

      // ident
      else if( isalpha(ch) || ch == '_' ) {
        cur->kind = TokenKind::Ident;
        while( isalnum(ch = peek()) || ch == '_' ) position++;
      }

      // char, string
      else if( ch == '"' || ch == '\'' ) {
        auto begin = ch;

        cur->kind = begin == '"' ? TokenKind::String : TokenKind::Char;

        for( position++; (ch = peek()) != begin; position++ );
        position++;

        cur->str = { str + 1, position - pos - 2 };
        pass_space();
        continue;
      }
      
      else {
        cur->kind = TokenKind::Punctuator;

        for( std::string_view s : long_punctuaters ) {
          if( match(s) ) {
            position += s.length();
            cur->str = s;
            goto found_op;
          }
        }

        for( char const& c : punctuaters ) {
          if( ch == c ) {
            position++;
            cur->str = { &c, 1 };
            goto found_op;
          }
        }

        Error::add_error(ErrorKind::InvalidToken, position, "invalid token");

      found_op:
        pass_space();
        continue;
      }

      cur->str = { str, position - pos };
      pass_space();
    }

    cur = new Token(TokenKind::End, cur, position);
    return top.next;
  }
}