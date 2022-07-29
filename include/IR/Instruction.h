#pragma once

#include <vector>
#include "AST.h"

namespace Metro {
  struct Object;
}

namespace Metro::IR {
  enum class IKind {
    None,
    Imm,
    Call,
    Define,
    List,
  };

  struct Instruction {
    IKind   kind = IKind::None;

    size_t            index = 0;
    std::string_view  str;
  };

}