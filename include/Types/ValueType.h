#pragma once

#include <string>
#include <vector>

#define BIT(N)  (1 << N)

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

    enum Attributes : uint8_t {
      ATTR_NONE,
      ATTR_LVALUE = BIT(1),
      ATTR_RVALUE = BIT(2),
      ATTR_CONST  = BIT(3),
      ATTR_REFERENCE = BIT(4),
      ATTR_CONSTREF  = ATTR_CONST | ATTR_REFERENCE,
    };

    Kind        kind;
    uint8_t     attr;
    std::vector<ValueType>   elems;

    ValueType(Kind kind = Kind::None)
      : kind(kind),
        attr(ATTR_NONE)
    {
    }

    bool have_extensions() const {
      return attr != ATTR_NONE;
    }

    bool equals_kind(Kind kind) const {
      return !have_extensions() && this->kind == kind;
    }

    bool equals(Kind kind) const;
    bool equals(ValueType const& type) const;

    std::string to_string() const;
  };
}