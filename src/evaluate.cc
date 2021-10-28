#include "common.h"
#include "token.h"
#include "object.h"
#include "AST.h"
#include "builtin_func.h"
#include "error.h"
#include "list_wrap.h"
#include "mangle.h"
#include "utils.h"

ObjectType evaluate(Node* node);

Cache<ObjectType> evaluated_caches;

extern BuiltinFunc const builtin_func_list[];
extern u64 const builtin_func_list_size;

namespace {
  ListWrap<Node*> scope_list;

  //
  std::map<Node*, bool> let_stmt_map;

  //
  // Cache<ObjectType> data_caches;

  // pair< name, data >
  // using data_pair = std::pair<std::string, Node*>;
  std::map<std::string, Node*> data_caches;
  std::map<std::string, Node*> func_caches;
  std::vector<Node*> data_list;

  bool check_list_in_data(Node* data, std::vector<std::pair<std::string, ObjectType>> const& list) {
    for (auto&& pair : list) {
      for (auto&& member : data->data->member_list) {
        if (std::get<0>(pair) == member->name) {
          auto&& _t = evaluated_caches.contains(member->type) ? evaluated_caches[member->type] : evaluate(member->type);

          if (std::get<1>(pair).equals(_t))
            goto cnt;
        }
      }

      return false;
    cnt:;
    }

    return true;
  }

  // std::map<Node*, ObjectType> evaluated_type_caches;

} // namespace

auto get_type_cache(Node* nd) {
  return evaluated_caches.contains(nd) ? evaluated_caches[nd] : evaluate(nd);
}

void evaluater_reset() {
  scope_list.clear();
  let_stmt_map.clear();

  data_caches.clear();
  func_caches.clear();
  data_list.clear();
}

//
// pair<Scope, Index>
std::pair<Node*, u32> find_var(std::string const& name, bool current_only = false) {
  if (current_only) {
    auto scope            = scope_list.get();
    auto const& variables = scope->var_list;

    for (u32 i = 0; i < variables.size(); i++) {
      auto const& var = variables[i];

      if (var.name == name) {
        return {scope, i};
      }
    }
  } else {
    for (auto&& scope : scope_list.list) {
      for (u32 i = 0; i < scope->var_list.size(); i++) {
        if (scope->var_list[i].name == name)
          return {scope, i};
      }
    }
  }

  return {};
}

bool check_lvalue(Node* node) {
  switch (node->kind) {
    case ND_VARIABLE:
      return true;

    case ND_INDEXREF:
    case ND_MEMBER_ACCESS:
      return check_lvalue(node->lhs);
  }

  return false;
}

bool is_branch_node(Node* node) {
  if (!node)
    return false;

  switch (node->kind) {
    case ND_IF:
      return true;
  }

  return false;
}

std::vector<Node*> get_last_node(Node* node) {
  using Vec = std::vector<Node*>;

  if (!node)
    return {};

  switch (node->kind) {
    case ND_FUNC:
    case ND_DATA:
      break;

    case ND_VALUE:
    case ND_VARIABLE:
    case ND_LET:
    case ND_NEW:
      return {node};

    case ND_IF:
      return {node->if_stmt->true_block, node->if_stmt->false_block};

    case ND_FOR:
      if (node->for_stmt->else_code)
        return {node->for_stmt->code, node->for_stmt->else_code};

      return {node->for_stmt->code};

    case ND_BLOCK: {
      Vec v;

      for (auto it = node->list.rbegin(); it != node->list.rend(); it++) {
        // v.emplace_back(*it);
        v.insert(v.begin(), *it);

        if (!is_branch_node(*it))
          break;
      }

      return v;
    }
  }

  return {};
}

int fmt_check(std::string const& str) {
  TODO_IMPL;
}

ObjectType evaluate(Node* node) {
  ObjectType last;

  if (!node)
    return {};
  if (evaluated_caches.contains(node))
    evaluated_caches[node];

  switch (node->kind) {
    case ND_VALUE:
      last = node->object.type;
      break;

    case ND_VARIABLE: {
      auto [scope, index] = find_var(node->token->getstr());

      if (!scope) {
        error(ERR_ERROR, node->token, "undefined variable");
        exit(1);
      }

      node->var_scope = scope;
      node->var_index = index;

      last = scope->var_list[index].type;
      break;
    }

    case ND_ARRAY: {
      if (node->list.empty()) {
        error(ERR_ERROR, node->token, "empty array is not valid");
        error(ERR_NOTE, node->token, "you can make empty array with " COL_PINK "array<T>()" COL_DEFAULT);

        break;
      }

      last = evaluate(node->list[0]);

      for (auto it = node->list.begin() + 1; it != node->list.end(); it++) {
        if (!last.equals(evaluate(*it)))
          error(ERR_ERROR, node->list[0]->token, "inconsistent array types");
      }

      last.arr_depth += 1;
      break;
    }

    case ND_MEMBER_ACCESS: {
      auto obj                = evaluate(node->lhs);
      auto const& member_list = obj.data_node->data->member_list;

      if (obj.kind != OBJ_DATA) {
        error(ERR_ERROR, node->token, "left side is not a data type");
      }

      Node* nd = node->rhs;
      while (nd->kind == ND_INDEXREF) {
        if (!evaluate(nd->rhs).equals(OBJ_INT))
          error(ERR_ERROR, nd->token, "subscript must be integer");

        nd = nd->lhs;
      }

      auto const& name = nd->token->getstr();

      for (i64 i = 0; i < member_list.size(); i++) {
        auto&& m = member_list[i];

        if (m->name == name) {
          last            = evaluate(m->type); // TODO: use cache
          node->var_index = i;

          goto done_member_access;
        }
      }

      error(ERR_ERROR, nd->token, "data '" + obj.data_node->data->name + "' is not have a member '" + name + "'");

    done_member_access:;

      break;
    }

    case ND_CALLFUNC: {
      auto const& name = node->token->getstr();
      std::vector<ObjectType> arg_types;
      // std::vector<Node*> matched_funcs;

      using MatchedFunc = std::tuple<Node*, std::string>; // <func node, mangled name>
      std::vector<MatchedFunc> matched_funcs;

      for (auto&& i : node->list) {
        arg_types.emplace_back(evaluate(i));
      }

      //
      /// find built-in function
      // for (auto&& func : builtin_func_list) {
      for (u64 i = 0; i < builtin_func_list_size; i++) {
        auto& func = builtin_func_list[i];

        if (func.name == name) {
          if (arg_types.size() < func.arg_types.size()) {
            error(ERR_ERROR, node->token, "too few arguments");
          } else if (!func.arg_free && arg_types.size() > func.arg_types.size()) {
            error(ERR_ERROR, node->token, "too many arguments");
          } else {
            for (u32 i = 0; i < arg_types.size(); i++) {
              if (i >= func.arg_types.size())
                break;
              if (!arg_types[i].equals(func.arg_types[i]))
                error(ERR_ERROR, node->list[i]->token, "type mismatch");
            }
          }

          node->callfunc->is_builtin   = true;
          node->callfunc->builtin_func = &func;
          return func.ret_type;
        }
      }

      // find match functions
      for (auto&& scope : scope_list.list) {
        for (auto&& item : scope->list) {
          if (item->kind == ND_FUNC && item->func->name == name) {
            // matched_funcs.emplace_back(item, item->func->get_name());

            matched_funcs.emplace_back(
              item, mangle(item->func->name, item->func->data_tag,
                      take_out_from_vector<std::string>(item->func->args, [](auto& x) { return x->name; })));

            /*
                        // if tagged
                        if (!item->func->data_tag.empty()) {
                          if (arg_types.empty())
                            error(ERR_ERROR, node->token,
                              "in call tagged function, should be have expression of " + item->func->data_tag +
                                " on first in arguments");
                          else if (arg_types[0].kind != OBJ_DATA || arg_types[0].data_node->data->name !=
               item->func->data_tag) error(ERR_ERROR, node->list[0]->token, "expected " + item->func->data_tag + " type
               expression");
                        }

                        if (arg_types.size() < item->func->args.size())
                          error(ERR_ERROR, node->token, "too few arguments");
                        else if (!item->func->arg_free && arg_types.size() > item->func->args.size())
                          error(ERR_ERROR, node->token, "too many arguments");
                        else {
                          for (u32 i = 0; i < arg_types.size(); i++) {
                            if (i >= item->func->args.size())
                              break;
                            if (!arg_types[i].equals(evaluated_caches[item->func->args[i]->type]))
                              error(ERR_ERROR, node->list[i]->token, "type mismatch");
                          }
                        }

                        node->callfunc->func_node = item;
                        return get_type_cache(item->func->code);
                        // return evaluated_caches[item->func->code];
                        */
          } // if matched name to item
        }
      }

      /*

      func
      func$<int,int>
      func@Point$<int,int>
     */

      // MatchedFunc* match;

      if (matched_funcs.size() > 1) {
        auto have_this = !arg_types.empty() && arg_types[0].kind == OBJ_DATA;

        for (i64 i = 0; i < matched_funcs.size();) {
          auto& match = matched_funcs[i];
          auto& fn    = std::get<0>(match);

          //
          // if (have_this && !fn->func->args.empty() && !get_type_cache(fn->func->args[0]->type).equals(arg_types[0]))
          // {
          //   matched_funcs.erase(matched_funcs.begin() + i);
          //   continue;
          // }

          // arguments are few than definition
          if (node->list.size() < fn->func->args.size()) {
            matched_funcs.erase(matched_funcs.begin() + i);
            continue;
          }

          // arguments are many than definition, but it is not allowed
          if (!fn->func->arg_free && node->list.size() > fn->func->args.size()) {
            matched_funcs.erase(matched_funcs.begin() + i);
            continue;
          }

          // check argument types
          for (i64 j = 0; j < fn->func->args.size(); j++) {
            if (!get_type_cache(fn->func->args[j]->type).equals(arg_types[j])) {
              matched_funcs.erase(matched_funcs.begin() + i);
              goto cnt_label_0;
            }
          }

          i++;
        cnt_label_0:;
        }

        if (matched_funcs.size() > 1) {
          error(ERR_ERROR, node->token, "too many matches found");
        }
      }

      if (matched_funcs.empty()) {
        error(ERR_ERROR, node->token, "undefined function");
        exit(1);
      }
      return get_type_cache((node->callfunc->func_node = std::get<0>(matched_funcs[0]))->func->code);
    }

    case ND_ARG:
      node = node->arg->type;

    case ND_TYPE: {
      auto const& name = node->type->name;
      ObjectType type;

      if (name == "int")
        type.kind = OBJ_INT;
      else if (name == "char")
        type.kind = OBJ_CHAR;
      else if (name == "double")
        type.kind = OBJ_DOUBLE;
      else if (name == "string")
        type.kind = OBJ_STRING;
      else if (name == "tuple")
        type.kind = OBJ_TUPLE;
      else {
        for (auto&& scope : scope_list.list) {
          for (auto&& item : scope->list) {
            if (item->kind == ND_DATA && item->data->name == name) {
              type.kind      = OBJ_DATA;
              type.data_node = item;
              goto found_data;
            }
          }
        }

        error(ERR_ERROR, node->token, "unknown type name");
      found_data:;
      }

      for (auto&& i : node->list) {
        type.elems.emplace_back(evaluate(i));
      }

      for (auto&& i : node->type->arr_depth) {
        evaluate(i);
      }

      type.arr_depth = node->type->arr_depth.size();

      evaluated_caches[node] = type;

      last = type;
      break;
    }

    case ND_ASSIGN: {
      if (node->lhs->kind == ND_ARRAY) {
        for (auto&& i : node->lhs->list)
          if (!check_lvalue(i))
            error(ERR_ERROR, i->token, "destination is not lvalue");
      } else if (!check_lvalue(node->lhs))
        error(ERR_ERROR, node->token, "left side is not lvalue");

      auto var_type  = evaluate(node->lhs);
      auto expr_type = evaluate(node->rhs);

      if (!var_type.equals(expr_type))
        error(ERR_ERROR, node->token, "type mismatch");

      last = var_type;
      break;
    }

    case ND_BLOCK: {
      scope_list.push(node);

      for (auto&& i : node->list) {
        if (i->kind == ND_LET)
          let_stmt_map[i] = true;

        last = evaluate(i);
      }

      scope_list.pop();
      break;
    }

    case ND_LET: {
      if (!let_stmt_map[node])
        error(ERR_ERROR, node->token, "cannot declare variable here");

      auto const& name = node->declvar->name;

      auto [scope, offs] = find_var(name, true);
      auto cur_scope     = scope_list.get();

      if (scope)
        error(ERR_ERROR, node->token, "multiple defined variable name");

      if (!node->declvar->init_expr && !node->declvar->type)
        error(ERR_ERROR, node->token, "cannot infer a type");

      auto type      = evaluate(node->declvar->type);
      auto init_type = evaluate(node->declvar->init_expr);

      if (node->declvar->type && node->declvar->init_expr && !type.equals(init_type))
        error(ERR_ERROR, node->token->next, "type mismatch");

      auto& type_n = node->declvar->type ? node->declvar->type : node->declvar->init_expr;

      auto& obj_type = node->declvar->type ? type : init_type;

      Object obj;
      obj.type = obj_type;
      obj.name = node->declvar->name;

      cur_scope->var_list.emplace_back(obj);

      node->var_scope = cur_scope;
      node->var_index = cur_scope->var_list.size() - 1;

      last = obj_type;
      break;
    }

    case ND_IF: {
      auto cond = evaluate(node->if_stmt->cond);

      if (cond.kind != OBJ_BOOL) {
        error(ERR_ERROR, node->token, "condition must be boolean");
      }

      auto true_type = evaluate(node->if_stmt->true_block);

      if (node->if_stmt->false_block && !true_type.equals(evaluate(node->if_stmt->false_block)))
        error(ERR_ERROR, node->token, "if-statement has inconsistent types");

      last = true_type;
      break;
    }

    case ND_FOR: {
      scope_list.push(node);

      if (node->for_stmt->begin->kind == ND_LET) {
        let_stmt_map[node->for_stmt->begin] = true;
      }

      evaluate(node->for_stmt->begin);

      if (!evaluate(node->for_stmt->cond).equals(OBJ_BOOL)) {
        error(ERR_ERROR, node->token, "condition must be boolean");
      }

      evaluate(node->for_stmt->count);
      evaluate(node->for_stmt->else_code);

      last = evaluate(node->for_stmt->code);
      scope_list.pop();

      break;
    }

    case ND_NEW: {
      using NameAndType = std::pair<std::string, ObjectType>;

      Node* data_node = nullptr;
      std::vector<NameAndType> init_list;
      std::vector<Node*> hits;

      for (auto&& i : node->newdata->initializer_list) {
        init_list.emplace_back(i->name, evaluate(i->expr));
      }

      if (!node->newdata->name.empty()) {
        for (auto&& scope : scope_list.list) {
          for (auto&& item : scope->list) {
            if (item->kind == ND_DATA && item->data->name == node->newdata->name) {
              if (check_list_in_data(item, init_list)) {
                node->newdata->data = item;
                data_node           = item;
                goto done;
              } else {
                error(ERR_ERROR, node->token, "invalid initializer list");
              }
            }
          }
        }

        error(ERR_ERROR, node->token, "data '" + node->newdata->name + "' is not found");
      }

      for (auto&& scope : scope_list.list) {
        for (auto&& item : scope->list) {
          if (item->kind == ND_DATA) {
            if (check_list_in_data(item, init_list)) {
              hits.emplace_back(item);
            }
          }
        }
      }

      if (hits.empty()) {
        error(ERR_ERROR, node->token, "does not found matches initializer list");
      } else if (hits.size() > 1) {
        error(ERR_ERROR, node->token, "matches too many data");

        for (auto&& i : hits) {
          error(ERR_NOTE, i->token, "matched to this");
        }

        error(ERR_NOTE, node->token->next, "maybe you forgot specify data name before this token ?");
        exit(1);
      }

      data_node           = *hits.begin();
      node->newdata->data = data_node;

    done:;
      // return evaluated_caches.contains(data_node) ? evaluated_caches[data_node] : evaluate(data_node);
      return get_type_cache(data_node);
    }

    case ND_FUNC: {
      // auto const& name = node->func->name;
      auto&& name = node->func->get_name();
      std::map<std::string, Node::Argument*> arg_map;

      auto& cache = func_caches[name];

      if (cache) {
        error(ERR_ERROR, node->token, "multiple defined function name");
        error(ERR_NOTE, cache->token, "defined here");
      } else {
        cache = node;
      }

      // check first arg if tagged
      if (!node->func->data_tag.empty()) {
        if (node->func->args.empty())
          error(ERR_ERROR, node->token, "tagged function is must be have 'this' argument on first");
        else {
          auto arg = node->func->args[0];
          auto&& t = get_type_cache(arg->type);

          if (t.kind != OBJ_DATA)
            error(ERR_ERROR, arg->token, "'this' must be data type");
          else if (t.data_node->data->name != node->func->data_tag)
            error(ERR_ERROR, arg->token->back,
              "no match data type for tag, must be specify '" + node->func->data_tag + "'");
        }
      }

      for (auto&& arg : node->func->args) {
        auto& xz = arg_map[arg->name];

        if (xz) {
          error(ERR_ERROR, arg->token, "cannot multiple use same name");
          error(ERR_NOTE, xz->token, "defined here");
        } else {
          Object obj;
          obj.type = evaluate(arg->type);
          obj.name = arg->name;

          node->var_list.emplace_back(obj);
          xz = arg;
        }
      }

      check_error(); // argument error

      evaluate(node->func->type);

      scope_list.push(node);

      evaluate(node->func->code);

      auto&& last_nodes = get_last_node(node->func->code);

      //
      /// Return Type Is Specified
      if (node->func->type) {
        auto& func_type = evaluated_caches[node->func->type];

        for (auto&& i : last_nodes) {
          auto const& type_c = get_type_cache(i);

          if (!func_type.equals(type_c)) {
            error(ERR_ERROR, i->token, "no match to returning type of function");

            error(ERR_NOTE, node->func->type->token, "declared here");
          }
        }

        last = func_type;
      }

      //
      /// Not Specified
      else if (!last_nodes.empty()) {
        auto first     = *last_nodes.begin();
        auto&& first_t = get_type_cache(first);

        for (auto it = last_nodes.begin() + 1; it != last_nodes.end(); it++) {
          if (!evaluated_caches[*it].equals(first_t)) {
            error(ERR_ERROR, (*it)->token, "no match to returning type of function");
            error(ERR_NOTE, first->token, "inferred as '" + first_t.to_string() + "' here");
          }
        }

        last = first_t;
      }

      scope_list.pop();
      break;
    }

    case ND_DATA: {
      std::map<std::string, Node::Argument*> name_map;
      auto chk = data_caches[node->data->name];

      if (chk != nullptr) {
        error(ERR_ERROR, node->token, "multiple defined data name");
        error(ERR_NOTE, chk->token, "defined here");
      }

      for (auto&& i : node->data->member_list) {
        auto& find    = name_map[i->name];
        auto member_t = evaluate(i->type);

        if (find) {
          error(ERR_ERROR, i->token, "multiple defined member name");
          error(ERR_NOTE, find->token, "defined here");
        }

        find = i;

        if (member_t.kind == OBJ_DATA && member_t.data_node == node) {
          error(ERR_ERROR, i->token->next->next, "cannot use incomplete type");
        }
      }

      last           = OBJ_DATA;
      last.data_node = node;

      data_caches[node->data->name] = node;
      data_list.emplace_back(node);

      break;
    }

    default: {
      auto lhs = evaluate(node->lhs);
      auto rhs = evaluate(node->rhs);

      if (!lhs.equals(rhs))
        error(ERR_ERROR, node->token, "type mismatch");

      switch (node->kind) {
        case ND_BIGGER:
        case ND_BIG_OR_EQ:
        case ND_SPACESHIP:
          if (!lhs.is_numeric())
            error(ERR_ERROR, node->token, "cannot compare not numeric type");

        case ND_EQUAL:
        case ND_NOT_EQUAL:
          lhs = node->kind == ND_SPACESHIP ? OBJ_INT : OBJ_BOOL;
          break;

        case ND_BIT_AND:
        case ND_BIT_XOR:
        case ND_BIT_OR:
        case ND_MOD:
          if (!lhs.equals(OBJ_INT))
            error(ERR_ERROR, node->token, "cannot use for non integer type");

          break;

        case ND_LOG_AND:
        case ND_LOG_OR:
          if (!lhs.equals(OBJ_BOOL))
            error(ERR_ERROR, node->token, "cannot use for non boolean type");

          lhs = OBJ_BOOL;
          break;
      }

      last = lhs;
      break;
    }
  }

  evaluated_caches[node] = last;
  return last;
}