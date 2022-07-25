#include "Types.h"
#include "Sema/Analyzer.h"

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
      case Kind::Value: {
        auto x = (AST::Value*)ast;
        auto val = new Object;

        switch( x->token->kind ) {
          case TokenKind::Int: {
            val->type = TypeKind::Int;
            val->v_int = atoi(ast->token->str.data());
            break;
          }
        }

        ret = val->type;
        x->object = val;
        break;
      }

      case Kind::Variable: {

        break;
      }

      case Kind::Scope: {
        
      }

      default: {
        auto x = (AST::Expr*)ast;

        ret = analyze(x->lhs);
        auto rhs = analyze(x->rhs);

        switch( x->kind ) {
          case Kind::Add: {

            break;
          }

          default:
            TODO_IMPL
        }

        break;
      }
    }

    return ret;
  }
}