#pragma once

#include <string>

namespace Metro {
  enum TypeKind {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_ARGS,
    TYPE_NONE
  };

  struct TypeInfo {
    TypeKind kind;

    TypeInfo(TypeKind kind = TYPE_NONE)
      : kind(kind)
    {
    }

    bool equals(TypeKind kind) const;
    bool equals(TypeInfo const& type) const;
    
    std::string to_string() const;
  };
}