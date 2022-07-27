#pragma once

#include <string>
#include <vector>

namespace Metro {
  struct ValueType {
    enum class Kind {
      Int,
      Float,
      Bool,
      Char,
      String,
      Tuple,
      Array,
      None
    };

    Kind      kind;
    bool      is_constant;
    bool      is_reference;
    std::vector<ValueType>   elems;

    ValueType(Kind kind = Kind::None)
      : kind(kind),
        is_constant(false),
        is_reference(false)
    {
    }

    bool have_extensions() const {
      return
        is_constant ||
        is_reference ||
        elems.size() != 0;
    }

    bool equals_kind(Kind kind) const {
      return !have_extensions() && this->kind == kind;
    }

    bool equals(Kind kind) const;
    bool equals(ValueType const& type) const;

    std::string to_string() const;
  };
}