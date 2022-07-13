#include "metro.h"

bool Evaluater::isAddable(TypeInfo left, TypeInfo right) {
  int i = 0;

  if( !left.equals(right) ) {
    return false;
  }

__re:
  switch( left.kind ) {
    case TYPE_INT:
    case TYPE_FLOAT:
      break;
    
    default:
      return false;
  }

  if( i == 0 ) {
    i++;
    left = right;
    goto __re;
  }

  return true;
}

TypeInfo Evaluater::eval(Node* node) {
  if( caches.contains(node) ) {
    return caches[node];
  }

  auto& ret = caches[node];

  switch( node->kind ) {
    case ND_VALUE: {
      auto obj = (node->object = new Object());

      switch( node->token->kind ) {
        case TOK_INT:
          obj->type = TYPE_INT;
          obj->v_int = atoi(node->token->str.data());
          break;

        case TOK_FLOAT:
          obj->type = TYPE_FLOAT;
          obj->v_float = atof(node->token->str.data());
          break;
        
        case TOK_CHAR:
        case TOK_STRING: {
          std::u16string s = Utils::Strings::to_wstring(std::string(node->token->str));

          if( node->kind == TOK_CHAR ) {
            if( s.length() == 0 || s.length() > 1 ) {
              crash;
            }

            obj->type = TYPE_CHAR;
            obj->v_char = s[0];
          }
          else {
            obj->type = TYPE_STRING;
            obj->v_str = std::move(s);
          }

          break;
        }

        default:
          crash;
      }

      ret = obj->type;
      break;
    }

    case ND_CALLFUNC: {


      break;
    }

    case ND_EXPR: {
      for( auto it = node->expr.begin() + 1; it != node->expr.end(); it++ ) {
        switch( it->kind ) {
          case EX_ADD:
            if( !isAddable(eval((it - 1)->node), eval(it->node)) ) {
              crash;
            }
            break;
        }
      }

      ret = eval(node->expr[0].node);
      break;
    }

    default: {
      crash;
    }
  }

  return ret;
}
