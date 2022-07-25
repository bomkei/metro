#include "Application.h"
#include "Types/Object.h"
#include "Utils.h"
#include "Debug.h"

namespace Metro {
  Object* Object::none = new Object(TypeKind::None);

  Object::Object(TypeInfo type)
    : type(type),
      ref_count(0),
      is_weak(false)
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
      case TypeKind::Int:
        ret = std::to_string(v_int);
        break;

      case TypeKind::Float:
        ret = std::to_string(v_float);
        break;

      case TypeKind::Bool:
        ret = v_bool ? "true" : "false";
        break;

      case TypeKind::Char:
        ret = Utils::Strings::to_string(std::u16string(v_char, 1));
        break;

      case TypeKind::String:
        ret = Utils::Strings::to_string(v_str);
        break;

      case TypeKind::None:
        ret = "none";
        break;
      
      default:
        TODO_IMPL
    }

    return ret;
  }
}