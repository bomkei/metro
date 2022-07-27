#pragma once

namespace Metro::AST {
  struct If : Base {
    Base*   cond;
    Base*   if_true;
    Base*   if_false;

    bool closed_with_else() const {
      auto x = if_false;

      if( !x ) {
        return false;
      }

      while( x && x->kind == Kind::If ) {
        x = ((AST::If*)x)->if_false;
      }

      return x != nullptr;
    }

    If(Token* tok)
      : Base(Kind::If, tok),
        cond(nullptr),
        if_true(nullptr),
        if_false(nullptr)
    {
    }
  };
}