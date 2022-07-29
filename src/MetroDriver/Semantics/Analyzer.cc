#include "Types.h"
#include "MetroDriver/Evaluator.h"
#include "MetroDriver/Semantics/Analyzer.h"
#include "Error.h"
#include "Debug.h"
#include "Utils.h"

namespace Metro::Semantics {
  using ASTKind = AST::Kind;

  Analyzer::Analyzer() {
    scope_list = new ScopeList;
  }

  void Analyzer::walk(AST::Base* ast) {
    switch( ast->kind ) {
      case ASTKind::None:
        break;

      case ASTKind::Value: {
        ((AST::Value*)ast)->object = create_obj(ast->token);
        break;
      }

      case ASTKind::Function: {
        auto x = (AST::Function*)ast;

        tmp_function.emplace_back(x, scope_list->clone());

        walk(x->code);

        break;
      }

      case ASTKind::Callfunc: {
        auto x = (AST::CallFunc*)ast;

        alert;

        tmp_callfunc.emplace_back(x, scope_list->clone());

        for( auto&& arg : x->args ) {
          walk(arg);
        }

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
          Error::add_error(ErrorKind::Undefined, ast->token, "undefiend function name");
        }
      }
      else {
        ast->callee = find;
      }
    }

  }

  AST::Function* Analyzer::find_func(WalkedTemp* tmp) {
    for( auto scope = tmp->scope; scope->scope; scope = scope->prev ) {
      alert;
      alertios("scope         " << scope);
      alertios("scope->scope  " << scope->scope);

      for( auto&& elem : scope->scope->elems ) {
        if( elem->kind == ASTKind::Function && ((AST::Function*)elem)->name == ((AST::CallFunc*)tmp->ast)->name ) {
          return (AST::Function*)elem;
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

  void Analyzer::enter_scope(AST::Scope* ast) {
    auto x = new ScopeList;

    x->scope = ast;
    x->prev = scope_list;

    scope_list = x;
  }

  void Analyzer::leave_scope() {
    auto x = scope_list;

    scope_list = scope_list->prev;

    delete x;
  }

}