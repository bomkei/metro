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
  
  static Token* create_name_token(std::string_view const& s) {
    auto tok = new Token(TOK_IDENT);

    tok->str = s;

    return tok;
  }

  static Node* ndmodfunc(BuiltinFunc const& func) {
    auto node = new Node(ND_BUILTIN_FUNC, nullptr);

    node->nd_builtin_func = &func;

    return node;
  }

  static Node* ndnamespace(std::string_view name, std::vector<Node*>&& elems) {
    auto node = new Node(ND_NAMESPACE, nullptr);

    node->nd_name = create_name_token(name);
    node->list = std::move(elems);

    return node;
  }

  Node* Application::construct_sysmodule() {
    auto node = ndnamespace("System", {
      ndnamespace("Console", {
        ndmodfunc(print),
        ndmodfunc(println),
      })
    });

    return node;
  }
}