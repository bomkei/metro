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

  void Application::initialize() {
    if( wasSysmoduleConstructed() ) {
      sysmodules_node = construct_sysmodule();
    }
  }

  int Application::main(int argc, char** argv) {
    
    

    auto obj = execScriptFile("test.txt");

    return 0;
  }
}