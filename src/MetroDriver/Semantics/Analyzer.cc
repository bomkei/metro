#include "Types.h"
#include "MetroDriver/Evaluator.h"
#include "MetroDriver/Semantics/Analyzer.h"
#include "Error.h"
#include "Debug.h"
#include "Utils.h"

namespace Metro::Semantics {
  using ASTKind = AST::Kind;

  Analyzer::Analyzer()
  {
  }

  void Analyzer::walk(AST::Base* ast) {
    switch( ast->kind ) {
      case ASTKind::None:
        break;

      case ASTKind::Value: {
        ((AST::Value*)ast)->object = create_obj(ast->token);
        break;
      }

      case ASTKind::Variable: {
        push_temp(tmp_variable, ast);
        break;
      }

      case ASTKind::Callfunc: {
        auto x = (AST::CallFunc*)ast;

        alert;

        push_temp(tmp_callfunc, x);
        //tmp_callfunc.emplace_back(x, scope_list->clone());

        for( auto&& arg : x->args ) {
          walk(arg);
        }

        break;
      }

      case ASTKind::Compare: {
        auto x = (AST::Compare*)ast;

        walk(x->first);

        for( auto&& item : x->list ) {
          walk(item.ast);
        }

        break;
      }

      case ASTKind::If: {
        auto x = (AST::If*)ast;

        walk(x->cond);
        walk(x->if_true);
        walk(x->if_false);

        break;
      }

      case ASTKind::Scope: {
        auto x = (AST::Scope*)ast;
        
        alert;
        alertios("Scope x = " << x);

        enter_scope(x);

        for( auto&& elem : x->elems ) {
          walk(elem);
        }

        leave_scope();

        break;
      }

      case ASTKind::Function: {
        auto x = (AST::Function*)ast;

        //tmp_function.emplace_back(x, scope_list);

        enter_scope(ast);

        walk(x->code);

        auto& tmp = push_temp(tmp_function, x);

        get_last_expr(tmp.last_expr, x->code);

        leave_scope();

        break;
      }

      default: {
        auto x = (AST::Expr*)ast;

        walk(x->lhs);
        walk(x->rhs);

        break;
      }
    }
  }

  void Analyzer::check_type_matches() {

  }

  void Analyzer::get_last_expr(std::vector<AST::Base*>& out, AST::Base* ast) {
    switch( ast->kind ) {
      case ASTKind::Type:
      case ASTKind::Argument:
      case ASTKind::Let:
        break;

      case ASTKind::For:
      case ASTKind::Loop:
      case ASTKind::While: {
        TODO_IMPL
      }

      case ASTKind::If: {
        auto x = (AST::If*)ast;

        get_last_expr(out, x->if_true);
        get_last_expr(out, x->if_false);

        break;
      }

      case ASTKind::Scope: {
        auto x = (AST::Scope*)ast;

        if( x->elems.empty() ) {
          out.emplace_back(ast);
        }
        else {
          get_last_expr(out, *x->elems.rbegin());
        }

        break;
      }

      case ASTKind::Function:
        break;

      default: {
        out.emplace_back(ast);
        break;
      }
    }
  }

  void Analyzer::analyze() {
    // check callee
    for( auto&& tmp : tmp_callfunc ) {
      auto ast = (AST::CallFunc*)tmp.ast;
      auto find = find_func(&tmp);

      if( find == nullptr ) {
        auto const& bifunlist = Evaluator::get_builtin_functions();

        for( auto&& bifun : bifunlist ) {
          if( bifun.name == ast->name ) {
            ast->callee_builtin = &bifun;
          }
        }

        if( !ast->callee_builtin ) {
          Error::add_error(ErrorKind::Undefined, ast->token, "undefined function name");
        }
      }
      else {
        ast->callee = find;
      }
    }

    for( auto&& tmp : tmp_variable ) {
      auto ast = (AST::Variable*)tmp.ast;
      auto defined = find_var_defined(&tmp);

      if( defined == nullptr ) {
        Error::add_error(ErrorKind::Undefined, ast->token, "undefined variable name");
      }
      else {
        ast->defined = defined;
      }
    }

  }

  AST::Function* Analyzer::find_func(WalkedTemp* tmp) {
    for( auto&& x : tmp->scope ) {
      if( x->kind == ASTKind::Scope ) {
        auto scope = (AST::Scope*)x;

        for( auto&& elem : scope->elems ) {
          if( elem->kind == ASTKind::Function && ((AST::Function*)elem)->name == ((AST::CallFunc*)tmp->ast)->name ) {
            return (AST::Function*)elem;
          }
        }
      }
    }

    return nullptr;
  }

  AST::Base* Analyzer::find_var_defined(WalkedTemp* tmp) {
    auto var = (AST::Variable*)tmp->ast;

    for( auto&& x : tmp->scope ) {
      switch( x->kind ) {
        case ASTKind::Scope: {
          auto scope = (AST::Scope*)x;

          for( auto&& elem : scope->elems ) {
            switch( elem->kind ) {
              case ASTKind::Let: {
                auto let = (AST::Let*)elem;

                if( let->name == var->name ) {
                  return let;
                }
              }
            }
          }

          break;
        }

        case ASTKind::Function: {
          auto fn = (AST::Function*)x;

          for( auto&& arg : fn->args ) {
            if( arg.name == var->name ) {
              return &arg;
            }
          }

          break;
        }
      }
    }

    return nullptr;
  }

  ValueType Analyzer::evaltype(AST::Base* ast) {
    ValueType ret;

    switch( ast->kind ) {
      case ASTKind::Value:
        return ((AST::Value*)ast)->object->type;

      default: {
        return evaltype(((AST::Expr*)ast)->lhs);
      }
    }

    return ret;
  }

  Object* Analyzer::create_obj(Token* token) {
    auto obj = new Object;

    switch( token->kind ) {
      case TokenKind::Int: {
        obj->type = ValueType::Kind::Int;
        obj->v_int = atoi(token->str.data());
        break;
      }

      case TokenKind::String: {
        obj->type = ValueType::Kind::String;
        obj->v_str = Utils::Strings::to_u16string(std::string(token->str));
        break;
      }

      default:
        crash;
    }

    return obj;
  }

}