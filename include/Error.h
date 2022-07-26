#pragma once

#include <vector>
#include "AppContext.h"

namespace Metro {
  struct Token;
  namespace AST {
    struct Base;
  }

  enum class ErrorKind {
    InvalidToken,
    InvalidSyntax,
    UninitializedValue,
    ExpectedToken,
    ExpectedSemicolon,
    NotAllowed,
    Undefined,
    IndefiniteType,
    UnknownTypeName,
    CannotRefer,
    ValueType,
    TypeMismatch,
    TooFewArguments,
    TooManyArguments,
    InvalidArguments,
    StackOverflow
  };

  struct ErrorContext {
    ErrorKind kind;

    Token*      token;
    AST::Base*  node;

    size_t  view_begin;
    size_t  view_end;
    std::string_view  src_view;

    size_t  err_pos;
    size_t  err_begin;
    size_t  err_end;

    size_t  err_underline_length;
    std::string message;

    AppContext::Script const* script;

    void show();
  };

  class Error {
  public:
    static void add_error(ErrorKind kind, size_t pos, std::string const& msg);
    static void add_error(ErrorKind kind, Token* token, std::string const& msg);
    static void add_error(ErrorKind kind, AST::Base* ast, std::string const& msg);

    static void show_all();

    static void check();

    [[noreturn]]
    static void exit_app();

    static std::vector<ErrorContext> const& get_ctx_list();

  private:
    static std::vector<ErrorContext> contexts;
  };
}