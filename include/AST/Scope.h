#pragma once

namespace Metro::AST {
  struct Scope : Base {
    std::vector<Base*>  elems;

    auto& append(Base* ast) {
      return elems.emplace_back(ast);
    }

    Scope(Token* tok)
      : Base(Kind::Scope, tok)
    {
    }
  };
}