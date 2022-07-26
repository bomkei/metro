#pragma once

namespace Metro::AST {
  struct Boolean : Base {
    bool const val;
    
    Boolean(Token* tok)
      : Base(Kind::Boolean, tok),
        val(tok->str == "true")
    {
    }
  };
}