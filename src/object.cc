#include "common.h"
#include "token.h"
#include "object.h"
#include "AST.h"

ObjectType::ObjectType(ObjectKind kin)
    : kind(kind)
    , arr_depth(0) {
}

ObjectType::ObjectType(ObjectType const&) = default;

ObjectType::ObjectType(ObjectType&& type) {
  this->operator=(std::move(type));
}

ObjectType& ObjectType::operator=(ObjectType const&) = default;

ObjectType& ObjectType::operator=(ObjectType&& type) {
  this->kind      = type.kind;
  this->arr_depth = type.arr_depth;
  this->elems     = std::move(type.elems);
  this->data_node = type.data_node;
  return *this;
}

bool ObjectType::is_numeric() const {
  switch (this->kind) {
    case OBJ_INT:
    case OBJ_DOUBLE:
      return true;
  }

  return false;
}

bool ObjectType::equals(ObjectType const& type) const {
  if (this->kind != type.kind || this->arr_depth != type.arr_depth)
    return false;

  if (this->kind == OBJ_TUPLE) {
    if (this->elems.size() != type.elems.size()) {
      for (u32 i = 0; i < this->elems.size(); i++) {
        if (!this->elems[i].equals(type.elems[i]))
          return false;
      }
    }
  }

  if (this->kind == OBJ_DATA)
    if (this->data_node != type.data_node)
      return false;

  return true;
}

bool ObjectType::cmp_data(Node* node) const {
  return this->kind == OBJ_DATA && this->data_node == node;
}

Object::Object(ObjectKind kind)
    : type(kind) {
}

Object::Object(ObjectType type)
    : type(type) {
}

Object::Object(Object const&) = default;

Object::Object(Object&& obj) {
  this->operator=(std::move(obj));
}

Object& Object::operator=(Object const&) = default;

Object& Object::operator=(Object&& obj) {
  this->type  = std::move(obj.type);
  this->name  = std::move(obj.name);
  this->v_str = std::move(obj.v_str);
  this->list  = std::move(obj.list);

  switch (this->type.kind) {
    case OBJ_INT:
      this->v_int = obj.v_int;
      break;

    case OBJ_CHAR:
      this->v_char = obj.v_char;
      break;

    case OBJ_DOUBLE:
      this->v_double = obj.v_double;
      break;

    case OBJ_BOOL:
      this->v_bool = obj.v_bool;
      break;
  }

  return *this;
}

Object Object::from_name(std::string const& name, ObjectType type) {
  Object obj = type;
  obj.name   = name;
  return obj;
}