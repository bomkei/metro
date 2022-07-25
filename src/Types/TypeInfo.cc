#include "Types/TypeInfo.h"
#include "Debug.h"

namespace Metro {
  bool TypeInfo::equals(TypeKind kind) const {
    return this->kind == kind;
  }

  bool TypeInfo::equals(TypeInfo const& type) const {
    if( kind != type.kind )
      return false;
    
    return true;
  }

  std::string TypeInfo::to_string() const {
    auto ret = std::string{ };

    switch( kind ) {
      case TypeKind::Int:
        ret = "int";
        break;

      case TypeKind::None:
        ret = "none";
        break;

      default: {
        TODO_IMPL
      }
    }

    if( is_constant ) {
      ret += " const";
    }

    if( is_reference ) {
      ret += "&";
    }

    return ret;
  }
}