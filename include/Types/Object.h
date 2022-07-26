#pragma once

#include <string>
#include "ValueType.h"
#include "Debug.h"

namespace Metro {
  struct Object {
    ValueType   type;
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

    Object(ValueType type = ValueType::Kind::None);
    ~Object();

    std::string to_string() const;

    Object* clone() const;
  };
}