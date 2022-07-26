#include "Types.h"
#include "Sema/Analyzer.h"
#include "Error.h"
#include "Utils.h"

namespace Metro::Sema {
  TypeInfo Analyzer::analyze_stmt(AST::Base* ast) {
    using AST::Kind;

    auto& ret = caches[ast];

    switch( ast->kind ) {
      case Kind::If: {
        auto x = (AST::If*)ast;
        auto cond = analyze(x->cond);

        if( !cond.equals_kind(TypeKind::Bool) ) {
          Error::add_error(ErrorKind::TypeMismatch, x->cond, "expected boolean expression as condition in if-statement");
        }

        ret = analyze(x->if_true);

        if( x->if_false && !ret.equals(analyze(x->if_false)) ) {
          Error::add_error(ErrorKind::TypeMismatch, ast->token,
            "type mismatch between true-case and false-case in if-statement.");
        }

        break;
      }

      case Kind::Let: {
        auto x = (AST::Let*)ast;

        if( !is_let_allowed ) {
          Error::add_error(ErrorKind::NotAllowed, ast->token, "let-statement is not allowed here");
        }
        else {
          is_let_allowed = false;
        }

        ret = analyze(x->type);
        break;
      }

      case Kind::Scope: {
        auto x = (AST::Scope*)ast;
        auto it = x->elems.begin();

        auto& pair = scope_history.emplace_front(ast, 0);

        for( ; it != x->elems.end() - 1; it++, pair.second++ ) {
          alert;
          analyze(*it);
        }

        alert;
        ret = analyze(*it);
        pair.second++;

        scope_history.pop_front();

        alertios("last value in scope = " << ret.to_string());
        break;
      }

      case Kind::Function: {
        auto x = (AST::Function*)ast;

        scope_history.emplace_back(ast, 0);

        for( auto&& arg : x->args ) {
          analyze(arg.type);
        }

        ret = analyze(x->return_type);

        if( !ret.equals(analyze(x->code)) ) {
          Error::add_error(ErrorKind::TypeMismatch, x->code, "type mismatch");
        }

        scope_history.pop_back();
        break;
      }
    }

    return ret;
  }
}