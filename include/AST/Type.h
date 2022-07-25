#pragma once

namespace Metro::AST {
  struct Type : Base {
    std::string_view    name;
    std::vector<Type*>  elems;
    bool  is_reference;
    bool  is_constant;

    Type(Token* tok)
      : Base(Kind::Type, tok),
        is_reference(false),
        is_constant(false)
    {
    }
  };
}