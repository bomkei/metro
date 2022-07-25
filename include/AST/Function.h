#pragma once

namespace Metro::AST {
  struct Function : Base {
    struct Argument {
      std::string_view  name;
      Type*             type;
    };

    std::string_view        name;
    std::vector<Argument>   args;
    Type*                   return_type;
    Scope*                  code;

    Function()
      : return_type(nullptr),
        code(nullptr)
    {
      this->kind = Kind::Function;
    }
  };
}