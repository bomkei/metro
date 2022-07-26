#include "Types.h"
#include "Sema/Analyzer.h"
#include "Error.h"
#include "Utils.h"

namespace Metro::Sema {
  TypeInfo Analyzer::analyze_expr(AST::Base* ast) {
    using AST::Kind;

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
        auto& ctx = get_cur_scope();

        if( !defined ) {
          Error::add_error(ErrorKind::Undefined, x->token, "undefined variable name");
          Error::exit_app();
        }
        else if( ctx.var_initialized_map[x->name].empty() ) {
          Error::add_error(ErrorKind::UninitializedValue, x->token, "uninitialized value");
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

      case Kind::Assign: {
        auto x = (AST::Expr*)ast;

        if( !is_lvalue(x->lhs) ) {
          Error::add_error(ErrorKind::ValueType, x->token, "lvalue required at left side");
          Error::exit_app();
        }

        if( !(ret = analyze(x->lhs)).equals(analyze(x->rhs)) ) {
          Error::add_error(ErrorKind::TypeMismatch, x->token, "type mismatch");
          Error::exit_app();
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