#include "AST.h"
#include "Types.h"
#include "Semantics/Analyzer.h"
#include "GC.h"
#include "Error.h"

namespace Metro::Semantics {
  TypeCon& Analyzer::walk_lval(AST::Base* ast) {
    switch( ast->kind ) {
      case ASTKind::Variable: {
        auto x = (AST::Variable*)ast;
        auto [scope, type] = find_var(x->name);

        if( !scope ) {
          Error::add_error(ErrorKind::Undefined, ast->token, "undefined variable name");
          Error::exit_app();
        }
        else {
          x->defined = type->defined;
        }

        return *type;
      }
    }

    throw 1;
  }
}