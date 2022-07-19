#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Application.h"

namespace Metro {
  static constexpr auto operator"" _view (char const* s, size_t len) {
    return std::string_view{ s, len };
  }

  bool Application::parse_args(int argc, char** argv) {
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
      else if( i <= argc - 2 && argv[i] == "--d_appstep"_view ) {
        ctx._d_max_step_to = atoi(argv[i + 1]);
        std::cout << "@debug: max app step is " << ctx._d_max_step_to << std::endl;
        i += 2;
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
  }
}