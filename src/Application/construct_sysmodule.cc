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

#define define_builtin_func(_Name,ret,_Args,__fn_) \
  static BuiltinFunc const _Name {.name=#_Name,.ret_type=ret, \
    .arg_types=_Args,.func=[](std::vector<Object*>const&args)->Object* __fn_};

namespace Metro {
  #include "builtin_defines.h"
  
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

  static Node* ndscope(std::vector<Node*>&& elems) {
    auto node = new Node(ND_SCOPE, nullptr);

    node->list = std::move(elems);

    return node;
  }

  Node* Application::construct_sysmodule() {
    auto node = ndscope({
      ndmodfunc(print),
      ndmodfunc(println),
    });

    return node;
  }
}