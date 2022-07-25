#pragma once

namespace Metro {
  struct Token;
}

namespace Metro::AST {
  struct Base {
    Kind    kind;
    Token*  token;
  };
}