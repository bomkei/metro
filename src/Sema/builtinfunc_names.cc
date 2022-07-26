#include <iostream>
#include "Debug.h"
#include "Types.h"
#include "Error.h"
#include "Sema/Analyzer.h"
#include "GC.h"

namespace Metro::Sema {
  std::vector<BuiltinFunc const> Analyzer::builtinfunc_list {
    #include "builtin_defines.h"
  };
}