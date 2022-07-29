#pragma once

#include "Instruction.h"
#include "CallStack.h"

namespace Metro {
  struct Object;
}

namespace Metro::IR {
  class Machine {
  public:
    Machine() { }

    Object* execute_inst(Instruction const& inst);

  private:

  };
}