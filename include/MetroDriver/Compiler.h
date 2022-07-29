#pragma once

#include <vector>
#include <memory>
#include "IR/Instruction.h"

namespace Metro {
  class Compiler {
  public:
    std::vector<IR::Instruction>  inst_list;

    void emit(char const* fmt, ...);

    void compile(AST::Base* ast);

  private:

  };
}