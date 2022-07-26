#pragma once

#include <map>
#include <list>

namespace Metro {
  struct TypeInfo;
  struct BuiltinFunc;
}

namespace Metro::AST {
  struct Base;
}

namespace Metro::Sema {
  class Analyzer {
  public:
    Analyzer() { }

    TypeInfo  analyze(AST::Base* ast);
    TypeInfo  analyze_expr(AST::Base* ast);
    TypeInfo  analyze_stmt(AST::Base* ast);

  private:
    struct ScopeContext {
      AST::Base*  scope;
      size_t      cur_index;

      std::map<std::string_view, std::vector<TypeInfo>> var_initialized_map;

      explicit ScopeContext(AST::Base* scope)
        : scope(scope),
          cur_index(0)
      {
      }
    };

    bool is_lvalue(AST::Base* ast);

    AST::Base* find_var_definition(std::string_view name, AST::Base* cur);
    AST::Function* find_function(std::string_view name);

    ScopeContext& get_cur_scope();
    ScopeContext& enter_scope(AST::Base* ast);
    void leave_scope();

    bool is_let_allowed = false;
    std::map<AST::Base*, TypeInfo> caches;
    std::list<ScopeContext> scope_history;

    static std::vector<BuiltinFunc> builtinfunc_list;
  };
}