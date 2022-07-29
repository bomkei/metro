#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Types.h"
#include "MetroDriver/Lexer.h"
#include "MetroDriver/Parser.h"
#include "MetroDriver/Semantics/Analyzer.h"
#include "MetroDriver/Evaluator.h"
#include "Application.h"
#include "Error.h"
#include "Debug.h"

namespace Metro {

  static void _print_token(Token* token) {
    for( ; token->kind != TokenKind::End; token = token->next ) {
      std::cerr << token->str << std::endl;
    }
  }

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
    auto const* ctx = Application::get_cur_appcontext();

    running_script.push_front(&script);

    Lexer lexer{ script.data };

    auto token = lexer.lex();

    Error::check();

    Parser parser{ token };

    auto ast = parser.parse();

    script.ast = ast;

    Error::check();

    Semantics::Analyzer analyzer;
    
    analyzer.walk(ast);
    analyzer.analyze();

    Error::check();

    Evaluator evaluator;

    auto obj = evaluator.eval(ast);

    debug(
      alert;
      std::cout << obj->to_string() << std::endl;
    )

    running_script.pop_front();

    return obj;
  }
}