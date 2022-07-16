#include "Types/TypeInfo.h"

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
    switch( kind ) {
      case TYPE_INT:
        return "int";
      
      case TYPE_NONE:
        return "none";
    }

    return "(unknown type)";
  }
}