#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Application.h"

namespace Metro {
  static constexpr auto HELP_STRING =
R"(usage: metro [options] <scripts>

Options:
  -h --help     : show help
  -v --version  : show version
  --no-print-filenames  : don't print file names while running multiple scripts
)";

  void Application::show_help() {
    std::cout << HELP_STRING;
  }
}