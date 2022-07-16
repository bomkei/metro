#include <iostream>
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

  AppContext::Script Application::open_script_file(char const* path) {
    std::ifstream ifs{ path };
    AppContext::Script script;

    if( ifs.fail() ) {
      std::cout << "cannot open file: " << path << std::endl;
      exit(1);
    }

    script.path = path;

    for( std::string line; std::getline(ifs, line); ) {
      script.data += line + '\n';
    }

    return script;
  }

  Object* Application::execute_script(AppContext::Script& script) {
    // std::string source;
    // std::ifstream ifs{ path };

    // // read source
    // for( std::string line; std::getline(ifs, line); ) {
    //   source += line + '\n';
    // }

    Lexer lexer{ script.data };

    auto token = lexer.lex();

    // print token
    // for(auto t=token;t->kind!=TOK_END;t=t->next){
    //   std::cout<<t->str<<std::endl;
    // }

    alert;

    Parser parser{ token };

    auto node = parser.parse();
    script.node = node;

    alert;

    Sema::Analyzer analyzer;
    auto type = analyzer.Check(node);
    alert;

    Evaluator eval;
    //auto obj = runner.run(node);

    alert;
    auto obj = eval.eval(node);

    alert;
    return obj;
  }

}