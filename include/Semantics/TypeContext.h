#pragma once

namespace Metro {
  struct BuiltinFunc;
}

namespace Metro::AST {
  struct Base;
}

namespace Metro::Semantics {
  struct TypeContext {
    enum class Condition {
      None,
      Inferred,
      UnknownTypeName,    // unknown type name
    };

    ValueType::Kind   kind;
    bool  is_constant;
    bool  is_reference;
    std::vector<TypeContext> elems;

    bool  may_notbe_evaluated = false;

    AST::Base*  ast;
    Condition   cond;

    std::vector<AST::Base*> inferred;
    std::vector<AST::Base*> canbe_last;

    AST::Base* defined;
    std::vector<AST::Base*> assignmented;

    bool equals_kind(ValueType::Kind kind) const {
      return !is_constant && !is_reference && this->kind == kind;
    }

    bool equals(TypeContext const& ctx) const {
      return
        kind == ctx.kind &&
        is_constant == ctx.is_constant &&
        is_reference == ctx.is_reference;
    }

    std::string to_string() const {

      return "";
    }

    TypeContext& operator = (ValueType::Kind kind) {
      this->kind = kind;
      this->is_constant = false;
      this->is_reference = false;

      return *this;
    }

    TypeContext(ValueType const& valtype = ValueType::Kind::None)
      : kind(valtype.kind),
        is_constant(valtype.is_constant),
        is_reference(valtype.is_reference),
        ast(nullptr),
        cond(Condition::None),
        defined(nullptr)
    {
    }
  };
}