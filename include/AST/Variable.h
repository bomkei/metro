#pragma once

#include <string_view>

namespace Metro {
  struct Object;
}

namespace Metro::AST {
  struct Variable : Base {
    std::string_view  name;
    Object*           value;
    
    Variable() {
      this->kind = Kind::Variable;
    }
  };
}