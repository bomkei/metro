#include <iostream>
#include "Types.h"
#include "MetroDriver/Evaluator.h"
#include "GC.h"

namespace Metro {
  #define make_bifun_lambda(e...) \
    [] (std::vector<Object*> const& args) -> Object* { e }

  #define make_bifun(_name,_ret_t,_args,_fp) \
    BuiltinFunc{ \
      .name = #_name, \
      .ret_type = ValueType::Kind:: _ret_t, \
      .arg_types = _args, \
      .func = _fp \
    }

  #define make_bifun_code(_name,_ret_t,_args,_code...) \
    make_bifun(_name,_ret_t,_args,make_bifun_lambda(_code))

  namespace {
    auto _bifun_print = make_bifun_lambda(
      auto ret = gcnew(ValueType::Kind::Int);

      for( auto&& arg : args ) {
        auto&& s = arg->to_string();

        ret->v_int += s.length();
        std::cout << s;
      }

      return ret;
    );
  }

  std::vector<BuiltinFunc> const Evaluator::builtin_funcs {
    make_bifun(print, Int, { ValueType::Kind::Array },
      _bifun_print
    ),

    make_bifun_code(println, Int, { ValueType::Kind::Array },
      auto&& ret = _bifun_print(args);

      std::cout << std::endl;
      ret->v_int += 1;

      return ret;
    )
  };

  std::vector<BuiltinFunc> const& Evaluator::get_builtin_functions() {
    return builtin_funcs;
  }
}