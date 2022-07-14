#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Sema/Analyzer.h"
#include "Utils.h"

namespace Metro::Sema {
  bool Analyzer::isAddable(TypeInfo left, TypeInfo right) {
    if( !left.equals(right) ) {
      return false;
    }

    switch( left.kind ) {
      case TYPE_INT:
      case TYPE_FLOAT:
        return true;
    }

    return false;
  }
}