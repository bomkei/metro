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
      if( cmp[i] == 0 ) break;
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