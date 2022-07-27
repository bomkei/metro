#include "AST.h"
#include "Types.h"
#include "MetroDriver/Compiler.h"
#include "Debug.h"

namespace Metro {

  using ASTKind = AST::Kind;
  using namespace IR;

  Instruction* Compiler::compile(AST::Base* ast) {
    if( !ast ) {
      alert;
      return;
    }

    auto ret = (Instruction*)nullptr;

    switch( ast->kind ) {
      case ASTKind::Value: {
        auto inst = new InstImm;
        auto value = new Object;
        ret = inst;

        switch( ast->token->kind ) {
          case TokenKind::Int: {
            value->type = ValueType::Kind::Int;
            value->v_int = atoi(ast->token->str.data());
            break;
          }
        }

        inst->value = value;
        break;
      }

      case ASTKind::Callfunc: {
        auto x = (AST::CallFunc*)ast;
        auto inst = new InstCall;
        ret = inst;

        inst->name = x->name;

        for( auto&& i : x->args ) {
          inst->args.emplace_back(compile(i));
        }

        break;
      }

      case ASTKind::Function: {
        auto x = (AST::Function*)ast;
        auto inst = new InstDefine;
        ret = inst;



        break;
      }

      case ASTKind::Scope: {
        auto x = (AST::Scope*)ast;
        auto inst = new InstList;
        ret = inst;

        for( auto&& elem : x->elems ) {
          inst->list.emplace_back(compile(elem));
        }

        break;
      }

      default: {

        break;
      }
    }

    return ret;
  }

}