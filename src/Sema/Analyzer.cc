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

      case Kind::Value: {
        auto x = (AST::Value*)ast;
        auto val = new Object;

        switch( x->token->kind ) {
          case TokenKind::Int: {
            val->type = TypeKind::Int;
            val->v_int = atoi(ast->token->str.data());
            break;
          }

          case TokenKind::String: {
            val->type = TypeKind::String;
            val->v_str = Utils::Strings::to_u16string(std::string(ast->token->str));
            break;
          }
        }

        x->object = val;
        ret = val->type;
        break;
      }

      case Kind::Variable: {
        auto x = (AST::Variable*)ast;
        auto defined = find_var_definition(x->name, ast);

        if( !defined ) {
          Error::add_error(ErrorKind::Undefined, x->token, "undefined variable name");
          Error::exit_app();
        }

        x->defined = defined;
        ret = analyze(defined);

        break;
      }

      case Kind::Callfunc: {
        auto x = (AST::CallFunc*)ast;
        auto const& name = x->name;

        auto find = find_function(x->name);
        x->callee = find;

        std::vector<TypeInfo>   call_arg_types, func_arg_types;

        for( auto&& arg : x->args ) {
          call_arg_types.emplace_back(analyze(arg));
        }

        if( !find ) {
          for( auto&& builtin : builtinfunc_list ) {
            if( builtin.name == name ) {
              func_arg_types = builtin.arg_types;
              ret = builtin.ret_type;
              x->callee_builtin = &builtin;
              goto _found_builtin;
            }
          }

          Error::add_error(ErrorKind::Undefined, ast->token, "undefined function name");
          Error::exit_app();
        _found_builtin:;
        }
        else {
          for( auto&& arg : find->args ) {
            func_arg_types.emplace_back(analyze(arg.type));
          }

          ret = analyze(find->return_type);
        }

        for( auto callarg_it = call_arg_types.begin(), callarg_end = call_arg_types.end();
          auto&& func_arg : func_arg_types ) {
          if( func_arg.equals_kind(TypeKind::Args) ) {
            break;
          }

          if( callarg_it == callarg_end ) {
            Error::add_error(ErrorKind::TooFewArguments, x->token, "too few arguments");
            goto _done_check_args;
          }

          if( !callarg_it->equals(func_arg) ) {
            Error::add_error(ErrorKind::TypeMismatch, x->token, "type mismatch");
          }
        }

      _done_check_args:;
        break;
      }

      case Kind::Compare: {
        auto x = (AST::Compare*)ast;
        ret = analyze(x->first);

        for( auto&& item : x->list ) {
          analyze(item.ast);
        }

        ret = TypeKind::Bool;
        break;
      }

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