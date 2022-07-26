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
      case Kind::BuiltinFunction:
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
        }

        break;
      }
    }

    return ret;
  }
}