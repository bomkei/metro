#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Lexer.h"
#include "Parser.h"
#include "Sema/Analyzer.h"
#include "Evaluator.h"
#include "Application.h"
#include "Debug.h"

#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"

namespace Metro {

  static void _print_token(Token* token) {
    for( ; token->kind != TOK_END; token = token->next ) {
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

  #if METRO_DEBUG
    #define  _dfn_check_step(__n) \
      if( ctx->_d_max_step_to < __n ) return nullptr;
  #endif

  _dfn_check_step(1);
    Lexer lexer{ script.data };

    auto token = lexer.lex();

  #if METRO_DEBUG
    if( ctx->_d_print._df_tokens ) {
      alert;
      _print_token(token);
    }
  #endif

    alert;

  _dfn_check_step(2);
    Parser parser{ token };

    auto node = parser.parse();
    script.node = node;

  #if METRO_DEBUG
    if( ctx->_d_print._df_nodes ) {
      alert;
      std::cerr << Debug::node2s(node) << std::endl;
    }
  #endif

    alert;

  _dfn_check_step(3);
    Sema::Analyzer analyzer;
    auto type = analyzer.check(node);
    alert;

  #if METRO_DEBUG
    if( ctx->_d_print._df_sema_result ) {
      alert;
      std::cerr << type.to_string() << std::endl;
    }
  #endif

  _dfn_check_step(4);
    Evaluator eval;
    alert;

    auto obj = eval.eval(node);
    alert;

  #if METRO_DEBUG
    if( ctx->_d_print._df_evaluated_obj ) {
      alert;
      std::cerr << obj->to_string() << std::endl;
    }
  #endif

    return obj;
  }
}