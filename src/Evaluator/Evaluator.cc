#include "Types.h"
#include "Evaluator.h"
#include "GC.h"

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
        using CmpKind = AST::Compare::Item::Kind;

        ret = gcnew(TypeKind::Bool);

        auto x = (AST::Compare*)ast;
        bool res = false;

        Object const* lhs = eval(x->first);
        Object const* rhs = nullptr;

        for( auto it = x->list.begin(); it != x->list.end(); it++, lhs = rhs ) {
          rhs = eval(it->ast);

          switch( it->kind ) {
            case CmpKind::BiggerLeft:
              break;

            case CmpKind::BiggerRight:
              switch( lhs->type.kind ) {
                case TypeKind::Int:
                  res = lhs->v_int < rhs->v_int;
                  break;
              }
              break;
          }

          if( !res ) {
            break;
          }
        }

        ret->v_bool = res;
        break;
      }

      case Kind::If: {
        auto x = (AST::If*)ast;
        auto cond = eval(x->cond);

        if( cond->v_bool ) {
          ret = eval(x->if_true);
        }
        else if( x->if_false ) {
          ret = eval(x->if_false);
        }

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

          case Kind::Sub:
            switch( ret->type.kind ) {
              case TypeKind::Int:
                ret->v_int -= rhs->v_int;
                break;
            }
            break;

          case Kind::Mul:
            switch( ret->type.kind ) {
              case TypeKind::Int:
                ret->v_int *= rhs->v_int;
                break;
            }
            break;
        }

        break;
      }
    }

    return ret;
  }
}