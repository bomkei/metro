#include <cstring>
#include "Types/Token.h"
#include "MetroDriver/Lexer.h"
#include "Utils.h"

namespace Metro {
  Lexer::Lexer(std::string const& source)
    : source(source),
      position(0),
      length(source.length())
  {
  }
}