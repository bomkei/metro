#pragma once

namespace Metro::AST {
  struct Scope : Base {
    std::vector<Base*>  elems;

    Scope() {
      this->kind = Kind::Scope;
    }
  };
}