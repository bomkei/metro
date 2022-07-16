#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Application.h"

namespace Metro {
  static constexpr auto VERSION_STRING =
R"(Metro 0.0.1
)";

  void Application::show_version() {
    std::cout << VERSION_STRING;
  }
}