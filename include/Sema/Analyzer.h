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

  private:
    AST::Base* find_var_definition(std::string_view name, AST::Base* cur);
    AST::Function* find_function(std::string_view name);

    bool is_let_allowed = false;
    std::map<AST::Base*, TypeInfo> caches;
    std::list<std::pair<AST::Base*, size_t>> scope_history;

    static std::vector<BuiltinFunc const> builtinfunc_list;
  };
}