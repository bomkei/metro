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

  Object* Application::execScriptFile(std::string const& path) {
    std::string source;
    std::ifstream ifs{ path };

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
    auto type = analyzer.Check(node);
    alert;

    Evaluator eval;
    //auto obj = runner.run(node);

    alert;
    auto obj = eval.eval(node);

    return obj;
  }

}