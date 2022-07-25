#pragma once

#include <string_view>
#include <vector>

namespace Metro::AST {
  struct Type : Base {
    std::string_view    name;
    std::vector<Type*>  elems;
    bool  is_reference;
    bool  is_constant;

    Type()
      : is_reference(false),
        is_constant(false)
    {
      this->kind = Kind::Type;
    }
  };
}