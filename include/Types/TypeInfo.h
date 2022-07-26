#pragma once

#include <string>
#include <vector>

namespace Metro {
  enum class TypeKind {
    Int,
    Float,
    Bool,
    Char,
    String,
    Tuple,
    Vec,
    Args,
    None
  };

  struct TypeInfo {
    TypeKind  kind;
    bool      is_constant;
    bool      is_reference;
    std::vector<TypeInfo>   elems;

    TypeInfo(TypeKind kind = TypeKind::None)
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

    bool equals_kind(TypeKind kind) const {
      return !have_extensions() && this->kind == kind;
    }

    bool equals(TypeKind kind) const;
    bool equals(TypeInfo const& type) const;

    std::string to_string() const;
  };
}