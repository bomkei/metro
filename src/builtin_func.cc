#include "common.h"
#include "object.h"
#include "builtin_func.h"
#include "error.h"
#include "utils.h"

#define builtin_func_begin [](ObjectArgs & args, Token * token) -> Object

extern BuiltinFunc const builtin_func_list[] = {
  BuiltinFunc(
    "println", OBJ_NONE, {}, true,
    builtin_func_begin {
      for (auto&& i : args) {
        std::cout << i;
      }
      std::cout << std::endl;
      return {};
    }),

  BuiltinFunc(
    "format", OBJ_STRING, {OBJ_STRING}, true,
    builtin_func_begin {
      auto arg_it         = args.begin() + 1;
      auto const& arg_end = args.end();

      auto const& fmt = Utils::Strings::to_string(args[0].v_str);
      std::string s;

      for (i64 i = 0; i < fmt.length(); i++) {
        char ch = fmt[i];

        if (ch == '%') {
          if (arg_it == arg_end) {
            error(ERR_ERROR, token, "error");
            exit(1);
          }

          s += arg_it->to_string();
          arg_it++;
        } else if (ch == '\\') {
          s += fmt[++i];
        } else {
          s += ch;
        }
      }

      Object obj = OBJ_STRING;
      obj.v_str  = Utils::Strings::to_u16string(s);

      return obj;
    }),
};

extern u64 const builtin_func_list_size = std::size(builtin_func_list);