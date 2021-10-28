#include "common.h"
#include "AST.h"
#include "utils.h"
#include "mangle.h"

std::string Node::Function::get_name() const {
  return mangle(this->name, this->data_tag,
    take_out_from_vector<std::string>(this->args, [](auto& x) { return x->type->type_to_string(); }));
}

Object& Node::getvar() const {
  return var_scope->var_list[var_index];
}

std::string Node::type_to_string() const {
  std::string ret = this->type->name;

  if (!this->list.empty()) {
    ret += "<" + join_with_method(", ", list, [](auto& x) { return x->type_to_string(); }) + ">";
  }

  for (i64 i = 0; i < type->arr_depth.size(); i++)
    ret += "[]";

  return ret;
}