#pragma once

#include <string>

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

    TypeInfo(TypeKind kind = TypeKind::None)
      : kind(kind),
        is_constant(false),
        is_reference(false)
    {
    }

    bool equals(TypeKind kind) const;
    bool equals(TypeInfo const& type) const;
    
    std::string to_string() const;
  };
}