#pragma once

#include <vector>
#include "AST.h"

namespace Metro {
  struct Object;
}

namespace Metro::IR {
  enum class Kind {
    None,
    Imm,
    Call,
    Define,
    List,
  };

  struct Instruction {
    Kind    kind;

  protected:
    Instruction() :kind(Kind::None) { }
    virtual ~Instruction() { }
  };

  struct InstImm : Instruction {
    Object*   value;

    InstImm() {
      kind = Kind::Imm;
    }
  };

  struct InstCall : Instruction {
    std::string_view    name;
    std::vector<Instruction*> args;

    InstCall() {
      kind = Kind::Call;
    }
  };

  struct InstDefine : Instruction {
    std::string_view  name;
    std::vector<AST::Argument*> args;
    AST::Type* return_type;
    Instruction* code;

    InstDefine() {
      kind = Kind::Define;
    }
  };

  struct InstList : Instruction {
    std::vector<Instruction*> list;

    InstList() {
      kind = Kind::List;
    }
  };

}