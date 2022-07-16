#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Lexer.h"
#include "Parser.h"
#include "Sema/Analyzer.h"
#include "Evaluator.h"
#include "GC.h"
#include "Application.h"

#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"

#define define_builtin_func(_Name,ret,_Args,fn) \
  static BuiltinFunc const _Name {.name=#_Name,.ret_type=ret, \
    .arg_types=_Args,.func=[](std::vector<Object*>const&args)->Object*fn};

namespace Metro {
  static Application* _inst;
  Node* Application::sysmodules_node;

  define_builtin_func(print, TYPE_INT, { TYPE_ARGS }, {
    auto ret = gcnew(TYPE_INT);

    for( auto&& arg : args ) {
      auto&& s = arg->to_string();
      
      ret->v_int += s.length();
      std::cout << s;
    }

    return ret;
  })

  define_builtin_func(println, TYPE_INT, { TYPE_ARGS }, {
    auto ret = print.func(args);

    std::cout << std::endl;
    ret->v_int += 1;
    return ret;
  })

  static Node* ndmodfunc(BuiltinFunc const& func) {
    auto node = new Node(ND_BUILTIN_FUNC, nullptr);

    
  }

  static Node* ndmodules(std::string_view name, std::vector<Node*> const& elems) {

  }

  Node* Application::construct_sysmodule() {

  }
}