#define define_builtin_func(__name,__ret,__argtypes,__code) \
  BuiltinFunc{ \
    .name = #__name, \
    .ret_type = __ret, \
    .arg_types = __argtypes, \
    .func = [] (std::vector<Object*> const& args) -> Object* __code \
  }

define_builtin_func(print, TypeKind::Int, { TypeKind::Args }, {
  auto ret = gcnew(TypeKind::Int);

  for( auto&& arg : args ) {
    auto&& s = arg->to_string();
    
    ret->v_int += s.length();

    alert;
    std::cout << s;
  }

  alert;
  return ret;
}),

define_builtin_func(println, TypeKind::Int, { TypeKind::Args }, {
  auto ret = print.func(args);

  std::cout << std::endl;
  ret->v_int += 1;
  return ret;
}),