#include <cstring>
#include "Types/Token.h"
#include "MetroDriver/Lexer.h"
#include "Utils.h"

namespace Metro {
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
}