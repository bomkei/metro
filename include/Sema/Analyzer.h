#pragma once

#include <map>
#include <list>

namespace Metro {
  struct TypeInfo;
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
    std::map<AST::Base*, TypeInfo> caches;
    std::list<AST::Base*> scope_history;
  };
}