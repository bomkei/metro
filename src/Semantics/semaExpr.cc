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

      //
      // Call function
      case ND_CALLFUNC: {
        auto const& name = node->token->str;

        // find
        auto func_node = find_func(name);
        node->nd_callee = func_node;

        // not found
        if( !func_node ) {
          Error::add_error(ERR_UNDEFINED_FUNC, node->token, "function '" + std::string(name) + "' is not defined");
          Error::exit_app();
        }

        bool is_builtin = func_node->kind == ND_BUILTIN_FUNC;
        BuiltinFunc const* builtin = func_node->nd_builtin_func;

        std::vector<TypeInfo>   args;
        std::vector<TypeInfo>   func_args;

        for( auto&& arg : node->list ) {
          args.emplace_back(check(arg));
        }

        // get argument types of function
        if( !is_builtin ) { // user-defined
          for( auto&& x : func_node->list ) {
            func_args.emplace_back(check(x));
          }

          ret = check(func_node->nd_ret_type);
        }
        else { // built-in
          func_args = builtin->arg_types;
          ret = builtin->ret_type;
        }

        auto arg_iter       = args.begin();
        auto arg_iter_func  = func_args.begin();

        auto arg_nd   = node->list.begin();

        // check arguments
        for( ; ; arg_iter++, arg_iter_func++ ) {
          alert;

          if( arg_iter_func == func_args.end() ) {
            alert;
            if( arg_iter != args.end() ) {
              Error::add_error(ERR_NO_MATCH_ARGUMENTS, node->token, "too many arguments");
            }

            break;
          }
          else if( arg_iter == args.end() ) {
            alert;
            if( arg_iter_func != func_args.end() && !arg_iter_func->equals(TYPE_ARGS) ) {
              Error::add_error(ERR_NO_MATCH_ARGUMENTS, node->token, "too few arguments");
            }

            break;
          }

          // variadic arguments
          if( arg_iter_func->equals(TYPE_ARGS) ) {
            alert;
            break;
          }
          else if( !arg_iter->equals(*arg_iter_func) ) {
            alert;
            Error::add_error(ERR_TYPE_MISMATCH, (*arg_nd)->token, "type mismatch");
            break;
          }
        }

        if( !is_builtin ) {
          check(func_node);
        }

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