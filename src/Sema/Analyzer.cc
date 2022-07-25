#include "Types.h"
#include "Sema/Analyzer.h"
#include "Error.h"

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
      case Kind::Type: {
        auto x = (AST::Type*)ast;

        if( x->name == "int" ) {
          ret = TypeKind::Int;
        }

        break;
      }

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

      case Kind::Callfunc: {


        break;
      }

      case Kind::Scope: {
        auto x = (AST::Scope*)ast;
        auto it = x->elems.begin();

        scope_history.push_front(ast);

        for( ; it != x->elems.end() - 1; it++ ) {
          analyze(*it);
        }

        ret = analyze(*it);
        scope_history.pop_front();
        
        break;
      }

      case Kind::Function: {
        auto x = (AST::Function*)ast;

        for( auto&& arg : x->args ) {
          analyze(arg.type);
        }

        ret = analyze(x->return_type);

        if( !ret.equals(analyze(x->code)) ) {
          Error::add_error(ErrorKind::TypeMismatch, x->code, "type mismatch");
        }

        break;
      }

      default: {
        auto x = (AST::Expr*)ast;

        ret = analyze(x->lhs);
        auto rhs = analyze(x->rhs);

        switch( x->kind ) {
          case Kind::Add: {

            break;
          }
        }

        break;
      }
    }

    return ret;
  }
}