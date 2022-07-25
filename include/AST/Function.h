#pragma once

namespace Metro::AST {
  struct Function : Base {
    std::string_view        name;
    std::vector<Argument>   args;
    Type*                   return_type;
    Scope*                  code;

    Function(Token* token)
      : Base(Kind::Function, token),
        return_type(nullptr),
        code(nullptr)
    {
    }
  };
}