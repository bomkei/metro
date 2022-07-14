#pragma once

/* -------------------- //
  BuiltinFunc


// --------------------- */

#include <vector>
#include "Object.h"

namespace Metro {
  struct BuiltinFunc {
    using FuncPointer = Object*(*)(std::vector<Object*> const& args);

    char const*   name;
    TypeInfo  ret_type;
    std::vector<TypeInfo>  arg_types;
    FuncPointer const func;
  };
}