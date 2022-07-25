#pragma once

namespace Metro {
  struct Object;
}

namespace Metro::AST {
  struct Variable : Base {
    std::string_view  name;
    Base*             defined;

    Variable()
      : defined(nullptr)
    {
      this->kind = Kind::Variable;
    }
  };
}