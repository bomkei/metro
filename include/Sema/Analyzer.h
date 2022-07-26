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
      NotAnalyzed,
      Unknown,    // unknown type name
      Inferred,
    };

    AST::Base*  ast;
    Condition   cond;
    ValueType   type;

    std::vector<AST::Base*> inferred;
    std::vector<AST::Base*> canbe_last;

    TypeContext(ValueType::Kind kind = ValueType::Kind::None)
      : ast(nullptr),
        cond(Condition::NotAnalyzed),
        type(kind)
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

    void check_symbols();

  

  private:

    std::pair<ScopeContext*, AST::Base*> find_var(std::string_view name);

    ScopeContext& get_cur_scope();

    std::map<AST::Base*, TypeContext> caches;

    std::list<AST::Base*> walking;
    std::list<ScopeContext> scope_history;
  };
}