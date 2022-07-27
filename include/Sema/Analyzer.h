#pragma once

#include <map>
#include <list>
#include "Types/ValueType.h"

namespace Metro {
  struct BuiltinFunc;
}

namespace Metro::AST {
  struct Base;
}

namespace Metro::Sema {
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

  struct ScopeContext {
    AST::Base*  ast;
    size_t      cur_index = 0;

    std::map<std::string_view, TypeContext>  variable_types;
  };

  class Analyzer {
  public:
    TypeContext walk(AST::Base* ast);
    TypeContext& walk_lval(AST::Base* ast);

    void check_symbols();

  

  private:

    void append_assign(TypeContext& type, AST::Base* ast);

    std::tuple<ScopeContext*, TypeContext*> find_var(std::string_view name);

    ScopeContext& get_cur_scope();

    std::map<AST::Base*, TypeContext> caches;

    std::list<AST::Base*> walking;
    std::list<AST::Base*> scopelist;
    std::map<AST::Base*, ScopeContext> scopemap;
  };
}