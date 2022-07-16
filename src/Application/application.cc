#include <fstream>
#include "Utils.h"
#include "Lexer.h"
#include "Parser.h"
#include "Sema/Analyzer.h"
#include "Evaluator.h"
#include "Application.h"

#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"

namespace Metro {
  static Application* _inst;
  Node* Application::sysmodules_node;

  Application::Application() {
    _inst = this;
  }

  Application::~Application() {

  }

  Application* Application::get_instance() {
    return _inst;
  }

  bool Application::wasSysmoduleConstructed() {
    return sysmodules_node != nullptr;
  }

  Node* Application::get_sysmodules_node() {
    return sysmodules_node;
  }

  void Application::initialize() {
    if( !wasSysmoduleConstructed() ) {
      sysmodules_node = construct_sysmodule();
    }
  }

  int Application::main(int argc, char** argv) {
    for( auto&& script : ctx.scripts ) {
      execute_script(script);
    }

    return 0;
  }
}