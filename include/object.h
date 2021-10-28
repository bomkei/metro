#pragma once

enum ObjectKind
{
  OBJ_INT,
  OBJ_CHAR,
  OBJ_DOUBLE,
  OBJ_BOOL,
  OBJ_STRING,
  OBJ_TUPLE,
  OBJ_DATA,
  OBJ_NONE
};

struct Node;
struct ObjectType {
  ObjectKind kind;
  u32 arr_depth;
  std::vector<ObjectType> elems;
  Node* data_node = nullptr;

  ObjectType(ObjectKind kind = OBJ_NONE);
  ObjectType(ObjectType const&);
  ObjectType(ObjectType&& type);

  ObjectType& operator=(ObjectType const&);
  ObjectType& operator=(ObjectType&& type);

  bool is_numeric() const;
  bool equals(ObjectType const& type) const;
  bool cmp_data(Node* node) const;
  std::string to_string() const;
};

struct Object {
  ObjectType type;
  std::string name;

  union {
    i64 v_int = 0;
    u16 v_char;
    double v_double;
    bool v_bool;
  };

  std::u16string v_str;
  std::vector<Object> list;

  Object(ObjectKind kind = OBJ_NONE);
  Object(ObjectType type);
  Object(Object const&);
  Object(Object&& obj);

  Object& operator=(Object const&);
  Object& operator=(Object&& obj);

  template <class objT>
  auto& push(objT&& x) {
    return list.emplace_back(x);
  }

  std::string to_string() const;

  static Object from_name(std::string const& name, ObjectType type = OBJ_NONE);
};
