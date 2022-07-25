#pragma once

define_builtin_func(print, TYPE_INT, { TYPE_ARGS }, {
  auto ret = gcnew(TYPE_INT);

  for( auto&& arg : args ) {
    auto&& s = arg->to_string();
    
    ret->v_int += s.length();

    alert;
    std::cout << s;
  }

  alert;
  return ret;
})

define_builtin_func(println, TYPE_INT, { TYPE_ARGS }, {
  auto ret = print.func(args);

  std::cout << std::endl;
  ret->v_int += 1;
  return ret;
})