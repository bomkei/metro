#pragma once

namespace Metro::AST {
  struct Argument : Base {
    std::string_view    name;
    Type*               type;
    Object*             value;

    std::pair<size_t, size_t> get_range_on_source() {
      return { token->pos, type->get_range_on_source().second };
    }

    Argument(Token* tok)
      : Base(Kind::Argument, tok),
        type(nullptr),
        value(nullptr)
    {
    }
  };
}