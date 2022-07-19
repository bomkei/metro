#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Application.h"

namespace Metro {
  static constexpr auto operator"" _view (char const* s, size_t len) {
    return std::string_view{ s, len };
  }

  template <size_t Len>
  static inline bool startswith(char const* s, char const (&cmp) [Len]) {
    for( size_t i = 0; auto&& c : cmp ) {
      if( i >= Len - 1 || s[i] == 0 ) break;
      else if( s[i] != c ) return false;
      i++;
    }

    return true;
  }

  bool Application::parse_args(int argc, char** argv) {
    #define _Invalid_arg goto _L_invalid_arg;

    ctx.argc = argc;
    ctx.argv = argv;
    ctx.app_name = *argv;

    for( int i = 1; i < argc; ) {
      std::string_view arg{ argv[i] };
      auto argraw = argv[i];

      if( arg == "-h" || arg == "--help" ) {
        show_help();
        return 0;
      }
      else if( arg == "-v" || arg == "--version" ) {
        show_version();
        return 0;
      }
      else if( arg == "--no-print-filename" ) {
        ctx.no_print_filenames = true;
        i++;
        continue;
      }
    #if METRO_DEBUG
      else if( arg == "--EXTRADEBUG" ) {
        ctx.__dbg_all_flag_bits = (uint32_t)-1;
        i++;
        continue;
      }
      else if( i <= argc - 2 && argv[i] == "--d_appstep"_view ) {
        ctx._d_max_step_to = atoi(argv[i + 1]);
        std::cout << "@debug: max app step is " << ctx._d_max_step_to << std::endl;
        i += 2;
        continue;
      }
      else if( startswith(argv[i], "--d_print=") ) {
        if( arg.length() == 10 ) {
          _Invalid_arg
        }

        for( size_t j = 10; j < arg.length(); j++ ) {
          switch( arg[j] ) {
            case 'q': ctx._d_print._df_tokens = 1; break;
            case 'w': ctx._d_print._df_nodes = 1; break;
            case 'e': ctx._d_print._df_sema_result = 1; break;
            case 'r': ctx._d_print._df_evaluated_obj = 1; break;
            case 't': ctx._d_print._df_construct_obj = 1; break;
            case 'y': ctx._d_print._df_destruct_obj = 1; break;
          }
        }

        i++;
        continue;
      }
    #endif
      else if( *argraw == '-' ) {
        std::cerr << "unknown argument" << std::endl;
        return 0;
      }
      else { // script
        ctx.scripts.emplace_back(open_script_file(argraw));
        i++;
      }
    }

    if( ctx.scripts.empty() ) {
      std::cout << "no script files." << std::endl;
      return 0;
    }

    return true;

  _L_invalid_arg:
    std::cerr << "invalid argument" << std::endl;
    return 0;
  }
}