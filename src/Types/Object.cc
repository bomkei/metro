#include "Application.h"
#include "Types/Object.h"
#include "Utils.h"
#include "Debug.h"

namespace Metro {
  Object* Object::none = new Object(ValueType::Kind::None);

  Object::Object(ValueType type)
    : type(type),
      ref_count(0),
      is_weak(false),
      v_int(0)
  {
  debug(
    alert;
    alertctor(Object);
  )
  }

  Object::~Object() {
  debug(
    alert;
    fprintf(stderr, "\t#Object destructing: %p\n", this);
  )
  }

  std::string Object::to_string() const {
    auto ret = std::string{ };

    switch( type.kind ) {
      case ValueType::Kind::Int:
        ret = std::to_string(v_int);
        break;

      case ValueType::Kind::Float:
        ret = std::to_string(v_float);
        break;

      case ValueType::Kind::Bool:
        ret = v_bool ? "true" : "false";
        break;

      case ValueType::Kind::Char:
        ret = Utils::Strings::to_string(std::u16string(v_char, 1));
        break;

      case ValueType::Kind::String:
        ret = Utils::Strings::to_string(v_str);
        break;

      case ValueType::Kind::None:
        ret = "none";
        break;
      
      default:
        TODO_IMPL
    }

    return ret;
  }

  Object* Object::clone() const {
    return new Object(*this);
  }
}