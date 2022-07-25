#pragma once

namespace Metro::AST {
  enum class Kind {
    None,
    Type,
    
    Value,
    Variable,
    Callfunc,

    Mul,
    Div,
    Add,
    Sub,
    Compare,

    If,
    Let,
    For,
    Loop,
    While,
    Scope,

    Function,
    BuiltinFunction,
  };
}