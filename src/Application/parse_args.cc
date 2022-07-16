#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Application.h"

namespace Metro {
  bool Application::parse_args(int argc, char** argv) {
    ctx.argc = argc;
    ctx.argv = argv;
    ctx.app_name = *argv;

    for( int i = 1; i < argc; i++ ) {
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
      else { // script
        ctx.scripts.emplace_back(open_script_file(argraw));
      }
    }

    if( ctx.scripts.empty() ) {
      std::cout << "no script files." << std::endl;
      return 0;
    }

    return true;
  }
}