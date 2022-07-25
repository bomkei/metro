#pragma once

namespace Metro::AST {
  struct Type;
  struct Let : Variable {
    Type*   type;

    Let() {
      this->kind = Kind::Let;
    }
  };
}