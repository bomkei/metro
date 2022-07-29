#include "Types.h"
#include "MetroDriver/Evaluator.h"
#include "GC.h"

namespace Metro {
  Object** Evaluator::eval_lvalue(AST::Base* ast) {
    using ASTKind = AST::Kind;

    switch( ast->kind ) {
      case ASTKind::Variable: {
        auto x = (AST::Variable*)ast;

        switch( x->defined->kind ) {
          case ASTKind::Argument:
            return &((AST::Argument*)x->defined)->value;

          case ASTKind::Let:
            return &((AST::Let*)x->defined)->value;
        }

        break;
      }
    }

    crash;
  }
}