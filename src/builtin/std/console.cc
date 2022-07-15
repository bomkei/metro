#include <iostream>
#include "BuiltinFunctions.h"
#include "GC.h"

#define def_builtin_func(_Name,ret,_Args,fn) \
  BuiltinFunc const _Name{.name=#_Name,.ret_type=ret, \
    .arg_types=_Args,.func=[](std::vector<Object*>const&args)->Object*fn};

namespace Metro::BuiltinFunctions {

  namespace Std::Console {
    def_builtin_func(print, TYPE_INT, { TYPE_ARGS }, {
      auto ret = gcnew(TYPE_INT);

      for( auto&& arg : args ) {
        auto&& s = arg->to_string();
        
        ret->v_int += s.length();
        std::cout << s;
      }

      return ret;
    })

    def_builtin_func(println, TYPE_INT, { TYPE_ARGS }, {
      auto ret = print.func(args);

      std::cout << std::endl;
      ret->v_int += 1;
      return ret;
    })
  }

}