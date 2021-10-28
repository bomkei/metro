#pragma once

#include "object.h"

struct BuiltinFunc {
  using FuncPointer = Object (*)(ObjectArgs&, Token*);

  char const* name;
  ObjectType ret_type;
  std::vector<ObjectType> arg_types;
  bool arg_free;
  FuncPointer func;

  BuiltinFunc(char const* name, ObjectType const& ret_type, std::initializer_list<ObjectType> const& arg_types,
    bool arg_free, auto func)
      : name(name)
      , ret_type(ret_type)
      , arg_types(arg_types)
      , arg_free(arg_free)
      , func(func) {
  }
};
