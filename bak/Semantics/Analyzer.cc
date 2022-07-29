#include "AST.h"
#include "Types.h"
#include "Semantics/Analyzer.h"
#include "GC.h"
#include "Error.h"

namespace Metro::Semantics {
  using ASTKind   = AST::Kind;
  using TypeCon   = TypeContext;

  Analyzer::Analyzer() {
    
  }
  
  Analyzer::~Analyzer() {
    
  }
}