#include "Application.h"
#include "Types/Object.h"
#include "Utils.h"

namespace Metro {
  Object* Object::none = new Object(TYPE_NONE);

  Object::Object(TypeInfo type)
    : type(type),
      ref_count(0),
      is_weak(false)
  {
  debug(
    auto app = Application::get_instance();

    if( app && Application::get_cur_appcontext()->_d_print._df_construct_obj ) {
      alert;
      alertctor(Object);
    }
  )
  }

  Object::~Object() {
  #if METRO_DEBUG
    auto app = Application::get_instance();

    if( app && Application::get_cur_appcontext()->_d_print._df_destruct_obj ) {
      alert;
      fprintf(stderr, "\t#Object destructing: %p\n", this);
    }
  #endif
  }

  std::string Object::to_string() const {
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
}