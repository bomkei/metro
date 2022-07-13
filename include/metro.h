#pragma once

#include <iostream>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <memory>
#include <list>
#include <map>

//#define  METRO_DEBUG  1
#define  METRO_DEBUG  (Application::get_instance()->is_debug_enabled())

#define  BIT(n)   (1 << n)

// #if METRO_DEBUG
//   #define alert     fprintf(stderr,"\t#alert %s:%d\n",__FILE__,__LINE__)
//   #define crash     { alert; fprintf(stderr,"\n\tcrashed!!\n"); exit(1); }
// #else
//   #define alert     0
//   #define crash     exit(1)
// #endif

namespace Utils {
  template <class... Args>
  std::string format(char const* fmt, Args&&... args) {
    static char buf[0x1000];
    sprintf(buf, fmt, args...);
    return buf;
  }

  class Strings {
    static inline std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
  
  public:
    static auto to_wstring(std::string const& s) {
      return conv.from_bytes(s);
    }

    static auto to_string(std::u16string const& s) {
      return conv.to_bytes(s);
    }
  };
}

enum TokenKind {
  TOK_INT,
  TOK_FLOAT,
  TOK_BOOL,
  TOK_CHAR,
  TOK_STRING,
  TOK_IDENT,
  TOP_OPERATOR,
  TOK_PUNCTUATER,
  TOK_END
};

struct Token {
  TokenKind kind;
  Token* prev;
  Token* next;
  std::string_view str;
  size_t pos;

  Token(TokenKind kind = TOK_INT)
    : kind(kind),
      prev(nullptr),
      next(nullptr),
      pos(0)
  {
  }

  Token(TokenKind kind, Token* prev, size_t pos)
    : kind(kind),
      prev(prev),
      next(nullptr),
      pos(pos)
  {
    if( prev ) prev->next = this;
  }
};

enum NodeKind {
  ND_TYPE,
  
  ND_VALUE,
  ND_VARIABLE,
  ND_CALLFUNC,

  ND_ADDR,

  ND_LET,
  ND_IF,
  ND_FOR,
  ND_FOREACH,
  ND_LOOP,
  ND_WHILE,
  ND_DO_WHILE,
  ND_EXPR,

  ND_FUNCTION,

  ND_CLASS,
};

enum ExprKind {
  EX_BEGIN,
  EX_ADD,
  EX_SUB,
  EX_MUL,
  EX_DIV,
};

struct Object;
struct Node {
  struct ExprItem {
    ExprKind kind;
    Node*    node;

    explicit ExprItem(ExprKind kind, Node* node)
      : kind(kind),
        node(node)
    {
    }
  };

  NodeKind    kind;
  Token*      token;
  Object*     object;

  Node*  owner;
  std::vector<Node*> nodes;
  std::vector<ExprItem> expr;

  Node*  code;

  Node(NodeKind kind, Token* token)
    : kind(kind),
      token(token),
      object(nullptr),
      owner(nullptr),
      code(nullptr)
  {
  }

  Node* append(Node* node) {
    return nodes.emplace_back(node);
  }

  ExprItem& expr_append(ExprKind kind, Node* node) {
    return expr.emplace_back(kind, node);
  }
};

enum TypeKind {
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_BOOL,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_NONE
};

struct TypeInfo {
  TypeKind kind;

  TypeInfo(TypeKind kind = TYPE_NONE)
    : kind(kind)
  {
  }

  bool equals(TypeKind kind) const {
    return this->kind == kind;
  }

  bool equals(TypeInfo const& type) const {
    if( kind != type.kind )
      return false;
    
    return true;
  }

  std::string to_string() const {
    switch( kind ) {
      case TYPE_INT:
        return "int";
      
      case TYPE_NONE:
        return "none";
    }

    return "(unknown type)";
  }
};

struct Object {
  TypeInfo    type;
  size_t      ref_count;
  bool        is_weak;

  union {
    int64_t     v_int;
    double      v_float;
    bool        v_bool;
    char16_t    v_char;
  };
  
  std::u16string  v_str;

  static Object* none;

  Object(TypeInfo type = TYPE_NONE)
    : type(type),
      ref_count(0),
      is_weak(false)
  {
  }

  std::string to_string() const {
    switch( type.kind ) {
      case TYPE_INT:
        return std::to_string(v_int);

      case TYPE_FLOAT:
        return std::to_string(v_float);

      case TYPE_BOOL:
        return v_bool ? "true" : "false";
      
      case TYPE_CHAR:
        return Utils::Strings::to_string(std::u16string(v_char, 1));
      
      case TYPE_STRING:
        return Utils::Strings::to_string(v_str);
      
      case TYPE_NONE:
        return "none";
    }

    return "(unknown type object)";
  }
};

struct BuiltinFunc {
  using FuncPointer = Object*(*)(std::vector<Object*> const& args);

  char const*   name;
  TypeInfo  ret_type;
  std::vector<TypeInfo>  arg_types;
  FuncPointer     func;
};

class Lexer {
public:
  explicit Lexer(std::string const& source);

  Token* lex();

private:
  bool check();
  char peek();
  bool match(std::string_view const& s);
  void pass_space();

  std::string const& source;
  size_t position;
  size_t const length;
};

class Parser {
public:
  explicit Parser(Token* token);

  Node* factor();
  Node* add();
  Node* mul();
  Node* expr();

  Node* func();

  Node* parse();

private:
  bool check();
  void next();
  bool eat(std::string_view const& str);
  void expect(std::string_view const& str);
  Node* makeexpr(Node* node);

  Node* scope();
  Node* scope_with_bracket();

  Token* cur;
  Token* ate;
};

class Evaluater {
public:
  Evaluater() { }

  TypeInfo eval(Node* node);

private:

  bool isAddable(TypeInfo left, TypeInfo right);


  std::map<Node*, TypeInfo> caches;
};

class NodeRunner {
public:
  NodeRunner() { }

  Object* clone(Object* obj);

  Object* run(Node* node);

  Object* objAdd(Object* left, Object* right);
  Object* objMul(Object* left, Object* right);

private:

};

class GC {
public:
  static void execute();
  static void stop();

  static Object* append(Object* obj);
};

class Application {
public:

  Application(bool const);

  int main(int argc, char** argv);

  bool is_debug_enabled();

  static Application* get_instance();

private:

  bool const _debug;
};

template <class... Args>
inline Object* gcnew(Args&&... args) {
  return GC::append(new Object(args...));
}

#define  alert  _Alert(__FILE__,__LINE__)
#define  crash  _Crash(__FILE__,__LINE__)

// debugs
inline static int _Alert(char const* f, int n) {
  return METRO_DEBUG ? fprintf(stderr,"\t#alert %s:%d\n",f,n) : 0;
}

inline static void _Crash(char const* f, int n) {
  fprintf(stderr,"crashed!! %s:%d\n",f,n);
  exit(1);
}

