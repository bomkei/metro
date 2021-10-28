#include "common.h"
#include "token.h"
#include "object.h"
#include "AST.h"
#include "utils.h"

using Utils::format;

std::string ObjectType::to_string() const {
  std::string str;

  switch (kind) {
    case OBJ_INT:
      str = "int";
      break;
    case OBJ_CHAR:
      str = "char";
      break;
    case OBJ_DOUBLE:
      str = "double";
      break;
    case OBJ_BOOL:
      str = "bool";
      break;
    case OBJ_STRING:
      str = "string";
      break;
    case OBJ_TUPLE:
      str = "tuple";
      break;
    case OBJ_NONE:
      str = "none";
      break;
    case OBJ_DATA:
      str = data_node->data->name;
      break;
  }

  if (!elems.empty()) {
    str += '<' + join(", ", elems) + '>';
  }

  for (u32 i = 0; i < arr_depth; i++)
    str += "[]";

  return str;
}

std::string Object::to_string() const {
  if (type.arr_depth) {
    return "[" + join(", ", list) + "]";
  }

  switch (type.kind) {
    case OBJ_INT:
      return std::to_string(v_int);
    case OBJ_CHAR:
      return Utils::Strings::to_string(std::u16string(1, v_char));
    case OBJ_DOUBLE:
      return std::to_string(v_double);
    case OBJ_BOOL:
      return v_bool ? "true" : "false";
    case OBJ_STRING:
      return Utils::Strings::to_string(v_str);

    case OBJ_TUPLE:
      return "(" + join(", ", list) + ")";

    case OBJ_DATA:
      return "{" + type.data_node->data->name + " " + join_with_method(", ", list, [](auto& x) {
        return x.name + ": " + ((x.type.kind == OBJ_STRING) ? ("\"" + x.to_string() + "\"") : x.to_string());
      }) + "}";

    case OBJ_NONE:
      return "none";
  }

  return "";
}

std::string node_to_string(Node* node) {
  std::string str;

  if (!node)
    return "";

  switch (node->kind) {
    case ND_VALUE:
      return node->object.to_string();

    case ND_VARIABLE:
      return node->token->getstr();

    case ND_CALLFUNC:
      str = node->token->getstr() + "(";

      for (u32 i = 0; i < node->list.size(); i++) {
        str += node_to_string(node->list[i]);
      }

    case ND_BLOCK: {
      str = "{ ";

      for (auto&& i : node->list) {
        str += node_to_string(i) + " ";
      }

      return str + "}";
    }

    case ND_IF:
      return "if " + node_to_string(node->if_stmt->cond) + " { " + node_to_string(node->if_stmt->true_block) +
             (node->if_stmt->false_block ? (" else {" + node_to_string(node->if_stmt->false_block) + "}") : "");

    case ND_LET:
      return "let " + node->declvar->name + (node->declvar->type ? (" : " + node_to_string(node->declvar->type)) : "") +
             (node->declvar->init_expr ? (" = " + node_to_string(node->declvar->init_expr)) : "");

    case ND_DATA:
      return "data " + node->data->name + " {" + (node->data->member_list.empty() ? "" : ({
        std::string s;
        for (u32 i = 0; i < node->data->member_list.size(); i++) {
          auto& mm = node->data->member_list[i];
          s += mm->name + ": " + node_to_string(mm->type);
          if (i < node->data->member_list.size() - 1)
            s += ", ";
        }
        s;
      })) + " }";

    case ND_TYPE:
      return node->type->name + (node->list.empty() ? "" : ({
        std::string s;
        for (u32 i = 0; i < node->list.size(); i++) {
          s += node_to_string(node->list[i]);
          if (i < node->list.size() - 1)
            s += ", ";
        }
        "<" + str + ">";
      })) + [](auto const& x) {
        std::string xs;
        for (auto&& i : x) {
          xs += "[" + node_to_string(i) + "]";
        }
        return xs;
      }(node->type->arr_depth);

    case ND_NEW:
      return "new { " + ({
        std::string s;
        for (u32 i = 0; i < node->newdata->initializer_list.size(); i++) {
          auto inix = node->newdata->initializer_list[i];
          s += inix->name + ": " + node_to_string(inix->expr);
          if (i < node->newdata->initializer_list.size() - 1)
            s += ", ";
        }
        str;
      }) + " }";
  }

  return node_to_string(node->lhs) + node->token->getstr() + node_to_string(node->rhs);
}
