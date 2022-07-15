#include <fstream>
#include "Utils.h"
#include "Lexer.h"
#include "Parser.h"
#include "Sema/Analyzer.h"
#include "Evaluator.h"
#include "Application.h"
#include "appInternal.h"

#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"

namespace Metro {
  static Application* _inst;

  Application::Application()
    : _app(new AppInternal)
  {
    _inst = this;
  }

  Application::~Application() {

  }

  Application* Application::get_instance() {
    return _inst;
  }

  int Application::main(int argc, char** argv) {
    
    auto obj = execScriptFile("test.txt");

    return 0;
  }
}