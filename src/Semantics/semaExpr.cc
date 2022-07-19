#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Types/BuiltinFunc.h"
#include "Sema/Analyzer.h"
#include "Error.h"
#include "Utils.h"

namespace Metro::Sema {
  TypeInfo Analyzer::expr(Node* node) {
    auto& ret = caches[node];

    switch( node->kind ) {
      case ND_VALUE: {
        auto obj = (node->nd_object = new Object());

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

        for( auto&& arg : node->list ) {
          arg_types.emplace_back(check(arg));
        }

        auto find = find_func(name);
        node->nd_callee = find;

        if( !find ) {
          Error::add_error(ERR_UNDEFINED_FUNC, node->token, "function '" + std::string(name) + "' is not defined");
          Error::exit_app();
        }

        // check args
        if( find->kind == ND_FUNCTION ) {
          ret = check(find->nd_ret_type);

          if( find->list.size() == arg_types.size() ) {
            for( auto x = find->list.begin(), y = node->list.begin(); auto&& z : arg_types ) {
              auto&& xx = check(*x);

              if( xx.kind == TYPE_ARGS ) {
                break;
              }

              if( !xx.equals(z) ) {
                Error::add_error(ERR_TYPE_MISMATCH, (*y++)->token, "type mismatch");
              }
            }
          }
          else { // no match args count
            goto _no_match_arg_count;
          }
        }
        else {
          BuiltinFunc const* bifun = find->nd_builtin_func;
          
          ret = bifun->ret_type;

          if( bifun->arg_types.size() == arg_types.size() ) {
            for( auto x = bifun->arg_types.begin(), y = arg_types.cbegin(); auto&& z : node->list ) {
              if( x->kind == TYPE_ARGS ) {
                break;
              }

              if( !(x++)->equals(*y++) ) {
                Error::add_error(ERR_TYPE_MISMATCH, z, "type mismatch");
              }
            }
          }
          else {
            goto _no_match_arg_count;
          }
        }

        break;

      _no_match_arg_count:;
        Error::add_error(ERR_NO_MATCH_ARGUMENTS, node->token, "no match arguments in call '" + std::string(name) + "'");
        break;
      }

      case ND_EXPR: {
        ret = check(node->expr[0].node);

        for( auto it = node->expr.begin() + 1; it != node->expr.end(); it++ ) {
          check(it->node);

          switch( it->kind ) {
            case EX_ADD:
              if( !isAddable(check((it - 1)->node), check(it->node)) ) {
                crash;
              }
              break;
          }
        }

        break;
      }

      case ND_SCOPE: {
        scope_history.push_front(node);

        for( auto&& node : node->list ) {
          check(node);
        }

        scope_history.pop_front();
        break;
      }
    }

    return ret;
  }
}