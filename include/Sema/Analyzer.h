#pragma once

namespace Metro::AST {
  struct Base;
}

namespace Metro::Sema {
  class Analyzer {
  public:
    Analyzer() { }

    TypeInfo  analyze(AST::Base* ast);

  private:
    
  };
}