#pragma once

struct Object {
  TypeInfo    type;
  size_t      ref_count;
  bool        is_weak;

  union {
    int64_t     v_int;
    double      v_float;
    bool        v_bool;
    char16_t    v_char;
  };
  
  std::u16string  v_str;

  static Object* none;

  Object(TypeInfo type = TYPE_NONE)
    : type(type),
      ref_count(0),
      is_weak(false)
  {
  }

  std::string to_string() const {
    switch( type.kind ) {
      case TYPE_INT:
        return std::to_string(v_int);

      case TYPE_FLOAT:
        return std::to_string(v_float);

      case TYPE_BOOL:
        return v_bool ? "true" : "false";
      
      case TYPE_CHAR:
        return Utils::Strings::to_string(std::u16string(v_char, 1));
      
      case TYPE_STRING:
        return Utils::Strings::to_string(v_str);
      
      case TYPE_NONE:
        return "none";
    }

    return "(unknown type object)";
  }
};
