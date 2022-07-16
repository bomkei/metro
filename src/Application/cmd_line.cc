#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Application.h"

namespace Metro {
  void Application::parse_args(int argc, char** argv) {
    ctx.app_name = *argv;

    for( int i = 1; i < argc; i++ ) {


    }

    if( ctx.scripts.empty() ) {
      std::cout
    }
  }
}