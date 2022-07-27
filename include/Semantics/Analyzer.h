#pragma once

#include <map>
#include <list>
#include "Types/ValueType.h"
#include "TypeContext.h"

namespace Metro::Semantics {
  class Analyzer {
    using TypeCon = TypeContext;

    struct ScopeContext {
      AST::Base*  ast;
      size_t      cur_index = 0;

      std::map<std::string_view, TypeContext>  variable_types;
    };

  public:
    Analyzer();
    ~Analyzer();

    TypeCon walk(AST::Base* ast);
    TypeCon& walk_lval(AST::Base* ast);

    TypeCon must_evaluated(AST::Base* ast);

    bool is_valid_operator(AST::Kind op_kind, TypeCon const& left, TypeCon const& right);

    void check_symbols();

  private:

    void append_assign(TypeCon& type, AST::Base* ast);

    std::tuple<ScopeContext*, TypeCon*> find_var(std::string_view name);
    AST::Function* find_func(std::string_view name);

    ScopeContext& get_cur_scope();


    std::list<AST::Base*> walking;
    std::list<AST::Base*> scopelist;
    std::map<AST::Base*, TypeCon> caches;
    std::map<AST::Base*, ScopeContext> scopemap;
  };
}