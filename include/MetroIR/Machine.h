#pragma once

#include "Instruction.h"
#include "CallStack.h"

namespace Metro::IR {
  class Machine {
  public:
    Machine() { }

    void execute_inst(InstructionBase& inst);

  private:

  };
}