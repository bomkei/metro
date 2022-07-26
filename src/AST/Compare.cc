#include "Types.h"
#include "AST.h"

namespace Metro::AST {
  Compare* Compare::create(Base*& ast) {
    if( ast->kind != AST::Kind::Compare ) {
      ast = new AST::Compare(ast);
    }

    return (Compare*)ast;
  }
}