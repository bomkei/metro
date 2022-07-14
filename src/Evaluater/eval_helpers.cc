#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Evaluater.h"
#include "Utils.h"

bool Evaluater::isAddable(TypeInfo left, TypeInfo right) {
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
