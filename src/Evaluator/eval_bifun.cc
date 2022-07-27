#include <iostream>
#include "Types.h"
#include "Evaluator.h"
#include "GC.h"

namespace Metro {

  #define make_bifun_lambda(e...) \
    [] (std::vector<Object*> const& args) -> Object* { e }

  #define make_bifun(_name,_ret_t,_args,_code ...) \
    BuiltinFunc{ \
      .name = #_name, \
      .ret_type = _ret_t, \
      .arg_types = _args, \
      .func = make_bifun_lambda(_code) \
    }

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
    make_bifun(println
  };

  std::vector<BuiltinFunc> const& Evaluator::get_builtin_functions() {
    return builtin_funcs;
  }

}