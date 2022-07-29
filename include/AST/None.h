#pragma once

namespace Metro::AST {
  struct None : Base {
    static None* val;

  private:
    None() { }
  };
}