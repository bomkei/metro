#pragma once

namespace Metro::AST {
  struct Function : Base {
    struct Argument {
      std::string_view  name;
      Type*             type;

      explicit Argument(std::string_view name, Type* type)
        : name(name),
          type(type)
      {
      }
    };

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