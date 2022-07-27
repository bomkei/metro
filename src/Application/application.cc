#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Lexer.h"
#include "Parser.h"
#include "Semantics/Analyzer.h"
#include "Evaluator.h"
#include "Application.h"

#include "Types.h"

namespace Metro {
  static Application* _inst;

  std::list<AppContext::Script*> Application::running_script;

  Application::Application() {
    _inst = this;
  }

  Application::~Application() {

  }

  Application* Application::get_instance() {
    return _inst;
  }

  AppContext const* Application::get_cur_appcontext() {
    return const_cast<AppContext const*>(&_inst->ctx);
  }

  AppContext::Script const* Application::get_running_script() {
    return *running_script.begin();
  }

  void Application::initialize() {
  }

  int Application::main(int argc, char** argv) {
    try {
      for( auto&& script : ctx.scripts ) {
        if( !ctx.no_print_filenames && ctx.scripts.size() > 1 ) {
          std::cout
            << script.path << std::endl;
        }

        execute_script(script);
      }
    }
    catch( ... ) {
      std::cerr
        << std::endl
        << "unhandled exception has been occurred." << std::endl;

      return -1;
    }

    return 0;
  }
}