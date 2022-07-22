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

      //
      // Variable
      case ND_VARIABLE: {
        auto const& name = node->nd_name->str;
        auto find = find_var_defined_node(name);

        // not found
        if( !find ) {
          Error::add_error(ERR_UNDEFINED, node->token, "undefined variable name");
          Error::exit_app();
        }
        else {
          node->nd_vardef = find;
        }

        ret = check(find);
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
          Error::add_error(ERR_UNDEFINED, node->token, "function '" + std::string(name) + "' is not defined");
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
          ret = check(func_node);
        }

        break;
      }

      case ND_EXPR: {
        ret = check(node->expr[0].node);
        Node* errnode;

        for( auto it = node->expr.begin() + 1; it != node->expr.end(); it++ ) {
          auto itval = check(it->node);

          switch( it->kind ) {
            case EX_ADD:
              if( !isAddable(ret, itval) ) {
                errnode = it->node;
                goto _typeMismatch;
              }
              break;
            
            case EX_SUB: {
              if( !ret.equals(itval) ) {
                errnode = it->node;
                goto _typeMismatch;
              }

              break;
            }

            case EX_BIG_L:
            case EX_BIG_R:
              switch( ret.kind ) {
                case TYPE_INT:
                  break;

                default:
                  goto _typeMismatch;
              }

              if( !ret.equals(itval) ) {
                goto _typeMismatch;
              }

              break;
          }

          ret = itval;
        }

        break;

      _typeMismatch:;
        Error::add_error(ERR_TYPE_MISMATCH, errnode->token, "type mismatch");
        Error::exit_app();
      }

      case ND_SCOPE: {
        if( node->list.empty() ) {
          node->kind = ND_NONE;
          break;
        }

        scope_history.push_front(node);

        for( size_t i = 0; i < node->list.size() - 1; i++ ) {
          check(node->list[i]);
        }

        auto last = *node->list.rbegin();

        if( last == nullptr ) {
          break;
        }

        ret = check(last);

        switch( last->kind ) {
          case ND_IF: {
            bool stopped_with_else = false;
            Node* nd = last;

            while( nd->nd_if_false ) {
              nd = nd->nd_if_false;

              if( nd->kind != ND_IF ) {
                stopped_with_else = true;
                break;
              }
            }

            if( !stopped_with_else ) {
              Error::add_error(ERR_INDEFINITE, last->token, "computer can't guess your choice");
              Error::exit_app();
            }

            break;
          }
        }

        scope_history.pop_front();
        break;
      }
    }

    return ret;
  }
}