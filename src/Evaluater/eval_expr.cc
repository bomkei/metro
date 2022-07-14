#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Evaluater.h"
#include "Utils.h"

TypeInfo Evaluater::eval_expr(Node* node) {
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
          std::u16string s = Utils::Strings::to_u16string(std::string(node->token->str));

          if( node->token->kind == TOK_CHAR ) {
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

    case ND_VARIABLE: {


      break;
    }

    case ND_CALLFUNC: {
      auto const& name = node->token->str;

      std::vector<TypeInfo> arg_types;

      for( auto&& arg : node->nodes ) {
        arg_types.emplace_back(eval(arg));
      }

      if( name == "println" ) {
        ret = TYPE_INT;
      }
      else {
        crash;
      }

      break;
    }

    case ND_EXPR: {
      ret = eval(node->expr[0].node);

      for( auto it = node->expr.begin() + 1; it != node->expr.end(); it++ ) {
        eval(it->node);

        switch( it->kind ) {
          case EX_ADD:
            if( !isAddable(eval((it - 1)->node), eval(it->node)) ) {
              crash;
            }
            break;
        }
      }

      break;
    }
  }

  return ret;
}
