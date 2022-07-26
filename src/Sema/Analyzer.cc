#include "Types.h"
#include "Sema/Analyzer.h"
#include "Error.h"
#include "Utils.h"

namespace Metro::Sema {
  TypeInfo Analyzer::analyze(AST::Base* ast) {
    using AST::Kind;

    if( !ast ) {
      return { };
    }
    else if( caches.contains(ast) ) {
      return caches[ast];
    }
  
    auto& ret = caches[ast];

    switch( ast->kind ) {
      case Kind::None:
        break;

      case Kind::Argument: {
        auto x = (AST::Argument*)ast;

        ret = analyze(x->type);

        break;
      }

      case Kind::Type: {
        auto x = (AST::Type*)ast;

        if( x->name == "int" ) {
          ret = TypeKind::Int;
        }
        else {
          Error::add_error(ErrorKind::UnknownTypeName, x->token, "unknown type name");
          Error::exit_app();
        }

        break;
      }

      case Kind::Value:
      case Kind::Variable:
      case Kind::Callfunc:
      case Kind::Compare:
      default:
        return analyze_expr(ast);

      case Kind::If:
      case Kind::Let:
      case Kind::Scope:
      case Kind::Function:
        return analyze_stmt(ast);
    }

    return ret;
  }
}