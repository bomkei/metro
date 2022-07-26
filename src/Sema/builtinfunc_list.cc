#include <iostream>
#include "Debug.h"
#include "Types.h"
#include "Error.h"
#include "Sema/Analyzer.h"
#include "GC.h"

#define bifun_lambda(e...) \
  [] (std::vector<Object*> const& args) -> Object* { e }

#define define_builtin_func_fp(__name,__ret,__argtypes,__fp) \
  BuiltinFunc{ \
    .name = #__name, \
    .ret_type = __ret, \
    .arg_types = __argtypes, \
    .func = __fp \
  }

#define define_builtin_func(__name,__ret,__argtypes,__func_code...) \
  define_builtin_func_fp(__name,__ret,__argtypes,bifun_lambda(__func_code))

namespace Metro::Sema {
  namespace {
    BuiltinFunc::FuncPointer bifun_print = bifun_lambda(
      auto ret = gcnew(TypeKind::Int);

      for( auto&& arg : args ) {
        auto&& s = arg->to_string();
        
        ret->v_int += s.length();

        alert;
        std::cout << s;
      }

      alert;
      return ret;
    );
  }

  std::vector<BuiltinFunc> Analyzer::builtinfunc_list {
    define_builtin_func_fp(print, TypeKind::Int, { TypeKind::Args }, bifun_print),

    define_builtin_func(println, TypeKind::Int, { TypeKind::Args },
      auto ret = bifun_print(args);

      std::cout << std::endl;
      ret->v_int += 1;
      return ret;
    ),
  };
}