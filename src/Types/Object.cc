#include "Types/Object.h"
#include "Utils.h"

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