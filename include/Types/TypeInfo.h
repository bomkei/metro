#pragma once

enum TypeKind {
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_BOOL,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_NONE
};

struct TypeInfo {
  TypeKind kind;

  TypeInfo(TypeKind kind = TYPE_NONE)
    : kind(kind)
  {
  }

  bool equals(TypeKind kind) const {
    return this->kind == kind;
  }

  bool equals(TypeInfo const& type) const {
    if( kind != type.kind )
      return false;
    
    return true;
  }

  std::string to_string() const {
    switch( kind ) {
      case TYPE_INT:
        return "int";
      
      case TYPE_NONE:
        return "none";
    }

    return "(unknown type)";
  }
};
