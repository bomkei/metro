#pragma once

#include <vector>
#include <memory>
#include "MetroIR/Instruction.h"

namespace Metro {
  class Compiler {
  public:
    std::vector<IR::Instruction*>  inst_list;

    IR::Instruction* compile(AST::Base* ast);

  private:

  };
}