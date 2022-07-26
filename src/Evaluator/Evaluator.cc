#include "Types.h"
#include "Evaluator.h"

namespace Metro {
  Object* Evaluator::eval(AST::Base* ast) {
    using AST::Kind;

    if( !ast ) {
      return Object::none;
    }

    Object* ret = Object::none;

    switch( ast->kind ) {
      case Kind::Function:
        break;

      case Kind::Value: {
        ret = ((AST::Value*)ast)->object;
        break;
      }

      case Kind::Variable: {
        auto x = (AST::Variable*)ast;

        switch( x->defined->kind ) {
          case Kind::Let:
            ret = ((AST::Let*)x->defined)->value;
            break;

          case Kind::Argument:
            ret = ((AST::Argument*)x->defined)->value;
            break;
        }

        break;
      }

      case Kind::Callfunc: {
        std::vector<Object*> args_bak;

        auto x = (AST::CallFunc*)ast;
        auto callee = x->callee;

        alert;

        for( size_t i = 0; i < x->args.size(); i++ ) {
          args_bak.emplace_back(callee->args[i].value);
          callee->args[i].value = eval(x->args[i]);
        }

        ret = eval(callee->code);

        for( size_t i = 0; i < x->args.size(); i++ ) {
          callee->args[i].value = args_bak[i];
        }

        break;
      }

      case Kind::Compare: {
        

        break;
      }

      case Kind::If: {


        break;
      }

      case Kind::Scope: {
        auto x = (AST::Scope*)ast;

        for( auto&& item : x->elems ) {
          ret = eval(item);
        }

        break;
      }

      default: {
        auto x = (AST::Expr*)ast;

        ret = eval(x->lhs)->clone();
        Object const* rhs = eval(x->rhs);

        switch( x->kind ) {
          case Kind::Add:
            switch( ret->type.kind ) {
              case TypeKind::Int:
                ret->v_int += rhs->v_int;
                break;
            }

            break;

          case Kind::Mul:
            switch( ret->type.kind ) {
              case TypeKind::Int:
                ret->v_int *= rhs->v_int;
                break;
            }
        }

        break;
      }
    }

    return ret;
  }
}