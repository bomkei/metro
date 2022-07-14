#include <fstream>
#include "Application.h"
#include "Lexer.h"
#include "Parser.h"
#include "Sema.h"
#include "Evaluator.h"
#include "Utils.h"

#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"

namespace Metro {
  static Application* _inst;

  Application::Application() {
    _inst = this;
  }

  Application* Application::get_instance() {
    return _inst;
  }

  int Application::main(int argc, char** argv) {

    std::string source;
    std::ifstream ifs{ "test.txt" };

    // read source
    for( std::string line; std::getline(ifs, line); ) {
      source += line + '\n';
    }

    Lexer lexer{ source };

    auto token = lexer.lex();

    // print token
    // for(auto t=token;t->kind!=TOK_END;t=t->next){
    //   std::cout<<t->str<<std::endl;
    // }

    alert;

    Parser parser{ token };

    auto node = parser.parse();

    alert;

    Sema::Analyzer analyzer;
    auto type = analyzer.Sema(node);
    alert;

    Evaluator eval;
    //auto obj = runner.run(node);
    auto obj = eval.eval(node->nodes[0]->code);

    //std::cout << obj->to_string() << std::endl;

    return 0;
  }
}