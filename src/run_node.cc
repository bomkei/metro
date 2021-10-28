#include "common.h"
#include "token.h"
#include "object.h"
#include "AST.h"
#include "builtin_func.h"
#include "error.h"
#include "list_wrap.h"

#define eval_obj(Lhs, Rhs, ...)                                                                                        \
  {                                                                                                                    \
    auto& __lhs = Lhs;                                                                                                 \
    auto& __rhs = Rhs;                                                                                                 \
    switch (__lhs.type.kind) { __VA_ARGS__ }                                                                           \
  }

#define object_operator(op, _Kind, _val_kind)                                                                          \
  case OBJ_##_Kind:                                                                                                    \
    __lhs.v_##_val_kind op## = __rhs.v_##_val_kind;                                                                    \
    break;

#define object_operator_bool(op, _Kind, _val_kind)                                                                     \
  case OBJ_##_Kind:                                                                                                    \
    __lhs.v_bool = __lhs.v_##_val_kind op __rhs.v_##_val_kind;                                                         \
    break;

#define object_operator_bool_ex(op, assign_op, _Kind, _val_kind)                                                       \
  case OBJ_##_Kind:                                                                                                    \
    __lhs.v_bool assign_op __lhs.v_##_val_kind op __rhs.v_##_val_kind;                                                 \
    break;

// extern Cache<ObjectType> evaluated_caches;
ObjectType get_type_cache(Node*);

struct FuncContext {
  Node* node;
  ObjectArgs args_bak;
  Object ret_value;
  // bool returned = false;
};

struct LoopContext {
  Node* node;
  bool breaked   = false;
  bool continued = false;
};

Object& assign_object(Object& dest, Object const& src);

namespace {
  ListWrap<FuncContext> func_list;
  ListWrap<LoopContext> loop_list;

  void enter_func(Node* func, ObjectArgs& args) {
    FuncContext ctx{};

    ctx.node     = func;
    ctx.args_bak = func->var_list;

    for (u32 i = 0; i < args.size(); i++) {
      assign_object(func->var_list[i], args[i]);
    }

    func_list.push(ctx);
  }

  void leave_func() {
    func_list.get().node->var_list = std::move(func_list.get().args_bak);

    func_list.pop();
  }

  void enter_loop(Node* node) {
    LoopContext ctx;
    ctx.node = node;

    loop_list.push(ctx);
  }

  void leave_loop() {
    loop_list.pop();
  }
} // namespace

// ObjectType get_type_cache(Node* node) {
//   return evaluated_caches.contains(node) ? evaluated_caches[node] : evaluate(node);
// }

Object& assign_object(Object& dest, Object const& src) {
  std::string name = dest.name;

  dest      = src;
  dest.name = std::move(name);

  return dest;
}

Object& run_lvalue(Node* node) {
  switch (node->kind) {
    case ND_VARIABLE:
      return node->getvar();
  }

  OOPS(0x1FB88A04);
}

Object& get_item_with_index(Object& obj, i64 index) {
  TODO_IMPL;
}

Object init_data_default(Node* node) {
  Object obj         = OBJ_DATA;
  obj.type.data_node = node;

  for (auto&& member : node->data->member_list) {
    auto&& type      = get_type_cache(member->type);
    auto const& name = member->name;

    if (type.kind == OBJ_DATA) {
      obj.push(init_data_default(type.data_node));
    } else {
      obj.push(Object::from_name(name, type));
    }
  }

  return obj;
}

Object run_node(Node* node) {
  Object last;

  if (!node)
    return {};

  switch (node->kind) {
    case ND_FUNC:
    case ND_DATA:
      break;

    case ND_VALUE:
      return node->object;
    case ND_VARIABLE:
      return run_lvalue(node);

    case ND_ARRAY: {
      Object obj;

      for (auto&& i : node->list)
        obj.list.emplace_back(run_node(i));

      obj.type = obj.list[0].type;
      obj.type.arr_depth++;

      return obj;
    }

    case ND_INDEXREF: {
      auto obj   = run_node(node->lhs);
      auto index = run_node(node->rhs);

      if (index.v_int < 0 || index.v_int >= obj.list.size())
        error(ERR_ERROR, node->token, "subscript out of range");

      return obj.list[index.v_int];
    }

    case ND_MEMBER_ACCESS: {
      auto obj = run_node(node->lhs);

      return obj.list[node->var_index];
    }

    case ND_CALLFUNC: {
      ObjectArgs args;
      auto const& name = node->token->getstr();

      for (auto&& i : node->list) {
        args.emplace_back(run_node(i));
      }

      if (node->callfunc->is_builtin) {
        return node->callfunc->builtin_func->func(args, node->token);
      } else {
        enter_func(node->callfunc->func_node, args);

        last = run_node(node->callfunc->func_node->func->code);

        leave_func();
      }

      return last;
    }

    case ND_NEW: {
      Object obj(init_data_default(node->newdata->data));

      for (u64 i = 0, j = 0; j < node->newdata->initializer_list.size() && i < obj.list.size(); i++) {
        auto init = node->newdata->initializer_list[j];

        if (obj.list[i].name == init->name) {
          assign_object(obj.list[i], run_node(init->expr));
          j++;
        }
      }

      return obj;
    }

    case ND_ASSIGN:
      if (node->lhs->kind == ND_ARRAY) {
        auto obj = run_node(node->rhs);

        if (obj.list.size() != node->lhs->list.size()) {
          error(ERR_ERROR, node->token, "no match destination count");
          exit(1);
        }

        for (std::size_t i = 0; i < obj.list.size(); i++) {
          assign_object(run_lvalue(node->lhs->list[i]), obj.list[i]);
        }
      }

      return assign_object(run_lvalue(node->lhs), run_node(node->rhs));

    case ND_BLOCK:
      for (auto&& i : node->list)
        last = run_node(i);

      return last;

    case ND_IF:
      if (run_node(node->if_stmt->cond).v_bool)
        return run_node(node->if_stmt->true_block);
      else if (node->if_stmt->false_block)
        return run_node(node->if_stmt->false_block);

      break;

    case ND_FOR: {
      enter_loop(node);

      run_node(node->for_stmt->begin);

      while (1) {
        auto cond = run_node(node->for_stmt->cond);
        auto& ctx = loop_list.get();

        if (!cond.v_bool)
          break;

        if (ctx.breaked)
          goto else_code;

        ctx.continued = false;
        last          = run_node(node->for_stmt->code);

        run_node(node->for_stmt->count);
      }

      leave_loop();
      return last;

    else_code:;
      leave_loop();
      return run_node(node->for_stmt->else_code);
    }

    case ND_LET:
      if (node->declvar->init_expr) {
        return assign_object(node->getvar(), run_node(node->declvar->init_expr));
      }

      return node->getvar();

    default:
      auto lhs = run_node(node->lhs);
      auto rhs = run_node(node->rhs);

      switch (node->kind) {
        case ND_ADD:
          eval_obj(lhs, rhs,
            object_operator(+, INT, int) object_operator(+, DOUBLE, double) object_operator(+, STRING, str)) break;

        case ND_SUB:
          eval_obj(lhs, rhs, object_operator(-, INT, int) object_operator(-, DOUBLE, double)) break;

        case ND_MUL:
          eval_obj(lhs, rhs, object_operator(*, INT, int) object_operator(*, DOUBLE, double)) break;

        case ND_DIV:
          eval_obj(lhs, rhs, object_operator(/, INT, int) object_operator(/, DOUBLE, double)) break;

        case ND_MOD:
          lhs.v_int %= rhs.v_int;
          break;

        case ND_BIGGER:
          eval_obj(lhs, rhs, object_operator_bool(>, INT, int) object_operator_bool(>, DOUBLE, double)) break;

        case ND_BIG_OR_EQ:
          eval_obj(lhs, rhs, object_operator_bool(>=, INT, int) object_operator_bool(>=, DOUBLE, double)) break;

        case ND_EQUAL:
          eval_obj(lhs, rhs, object_operator_bool(==, INT, int) object_operator_bool(==, DOUBLE, double)) break;

        case ND_NOT_EQUAL:
          eval_obj(lhs, rhs, object_operator_bool(!=, INT, int) object_operator_bool(!=, DOUBLE, double)) break;

        case ND_SPACESHIP:
          switch (lhs.type.kind) {
            case OBJ_INT:
              lhs.v_int = (lhs.v_int >= rhs.v_int) ? (lhs.v_int != rhs.v_int) : -1;
              break;

            case OBJ_DOUBLE:
              lhs.v_int = (lhs.v_double >= rhs.v_double) ? (lhs.v_double != rhs.v_double) : -1;
              break;
          }
          lhs.type = OBJ_INT;
          break;

        case ND_LOG_AND:
          lhs.v_bool &= rhs.v_bool;
          break;

        case ND_LOG_OR:
          lhs.v_bool |= rhs.v_bool;
          break;
      }

      return lhs;
  }

  return {};
}