#pragma once

#include <map>
#include <list>
#include "Types/ValueType.h"
#include "TypeContext.h"

namespace Metro::Semantics {
  struct ScopeContext {
    AST::Base*  ast;
    size_t      cur_index = 0;

    std::map<std::string_view, TypeContext>  variable_types;
  };

  class Analyzer {
    using TypeCon = TypeContext;

  public:
    TypeCon walk(AST::Base* ast);
    TypeCon& walk_lval(AST::Base* ast);

    bool is_valid_operator(AST::Kind op_kind, TypeCon const& left, TypeCon const& right);

    void check_symbols();

  private:

    void append_assign(TypeCon& type, AST::Base* ast);

    std::tuple<ScopeContext*, TypeCon*> find_var(std::string_view name);

    ScopeContext& get_cur_scope();

    std::map<AST::Base*, TypeCon> caches;

    std::list<AST::Base*> walking;
    std::list<AST::Base*> scopelist;
    std::map<AST::Base*, ScopeContext> scopemap;
  };
}