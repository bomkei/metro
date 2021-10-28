#pragma once

#include "object.h"

enum NodeKind
{
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_MOD,

  ND_LSHIFT,
  ND_RSHIFT,

  ND_BIGGER,
  ND_BIG_OR_EQ,
  ND_SPACESHIP,
  ND_EQUAL,
  ND_NOT_EQUAL,

  ND_BIT_AND,
  ND_BIT_OR,
  ND_BIT_XOR,

  ND_LOG_AND,
  ND_LOG_OR,

  ND_ASSIGN,

  ND_BLOCK,
  ND_LET,
  ND_IF,
  ND_FOR,
  ND_WHILE,
  ND_LOOP,

  ND_DATA,
  ND_FUNC,

  ND_ARG,
  ND_TYPE,
  ND_TYPE_ARR_DEPTH,

  ND_INDEXREF,
  ND_MEMBER_ACCESS,

  ND_NEW,

  ND_ARRAY,
  ND_VALUE,
  ND_VARIABLE,
  ND_CALLFUNC,
};

struct BuiltinFunc;
struct Node {
  struct Type {
    std::string name;
    // std::vector<Type*> elems;
    std::vector<Node*> arr_depth;
  };

  struct Argument {
    Node* type   = nullptr;
    Token* token = nullptr;
    std::string name;
  };

  struct Initializer {
    std::string name;
    Token* token;
    Node* expr;
  };

  struct Scope {
    // pair< name, type >
    using Variable = std::pair<std::string, Node*>;

    std::vector<Variable> variables;
  };

  struct DeclVar {
    std::string name;

    Node* type      = nullptr;
    Node* init_expr = nullptr;
  };

  struct If {
    Node* cond;
    Node* true_block;
    Node* false_block;
  };

  struct For {
    Node* begin     = nullptr;
    Node* cond      = nullptr;
    Node* count     = nullptr;
    Node* code      = nullptr;
    Node* else_code = nullptr;
  };

  struct Data {
    std::string name;
    std::vector<Argument*> member_list;
  };

  struct Function {
    std::string name;
    std::vector<Argument*> args;
    bool arg_free = false;

    Node* type = nullptr;
    Node* code = nullptr;

    std::string data_tag;

    std::string get_name() const;

    /*
    std::string get_mangled_name() const {
      std::string ret = this->name;

      if (!data_tag.empty())
        ret += "@" + data_tag;

      if (!this->args.empty()) {
        ret += "$<" + join_with_method(",", this->args, [](auto& x) { return x->type_to_string(); }) + ">";
      }

#if __DEBUG__
      alert;
      std::cout << ret << std::endl;
#endif

      return ret;
    }

    static std::string demangle(std::string const& str) {
      TODO_IMPL;
    }
    */
  };

  struct NewData {
    std::string name;
    Node* data = nullptr;
    std::vector<Initializer*> initializer_list;
  };

  struct CallFunc {
    bool is_builtin                 = false;
    Node* func_node                 = nullptr;
    BuiltinFunc const* builtin_func = nullptr;
  };

  NodeKind kind;
  Token* token;
  Object object;

  std::vector<Node*> list;
  std::vector<Object> var_list;

  Node* var_scope;
  u64 var_index;
  Scope* scope;

  // u64 member_index;

  union {
    struct {
      Node* lhs;
      Node* rhs;
    };

    Type* type = nullptr;
    Argument* arg;
    DeclVar* declvar;
    Data* data;
    If* if_stmt;
    For* for_stmt;
    Function* func;
    NewData* newdata;
    CallFunc* callfunc;
  };

  Object& getvar() const;
  std::string type_to_string() const;
};
