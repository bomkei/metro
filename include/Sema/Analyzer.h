#pragma once

#include <map>

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
  };
}