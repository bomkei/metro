#pragma once

#include <string>
#include "TypeInfo.h"
#include "Debug.h"

namespace Metro {
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

    Object(TypeInfo type = TypeKind::None);
    ~Object();

    std::string to_string() const;

    Object* clone() const;
  };
}