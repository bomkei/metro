#include <vector>
#include "Types.h"
#include "Application.h"
#include "Error.h"

namespace Metro {
  std::vector<ErrorContext> Error::contexts;

  void Error::add_error(ErrorKind kind, size_t pos, std::string const& msg) {
    auto& ctx = contexts.emplace_back();

    ctx.kind = kind;
    ctx.err_pos = pos;
    ctx.message = msg;

  }

  void Error::add_error(ErrorKind kind, Token* token, std::string const& msg) {
    auto& ctx = contexts.emplace_back();

    ctx.kind = kind;
    ctx.token = token;
    ctx.err_pos = token->pos;
    ctx.message = msg;

    auto script = Application::get_running_script();

    size_t err_view_begin = ctx.err_pos;
    size_t err_view_end = ctx.err_pos;

    while( err_view_begin > 0 ) {
      if( script->data[err_view_begin - 1] == '\n' ) {
        break;
      }

      err_view_begin--;
    }

    while( err_view_end < script->data.length() ) {
      if( script->data[err_view_end] == '\n' ) {
        break;
      }

      err_view_end++;
    }

    ctx.script = script;
    ctx.err_begin = err_view_begin;
    ctx.err_end = err_view_end;
    
    ctx.err_underline_length = token->str.length();
    
  }

  void Error::add_error(ErrorKind kind, AST::Base* ast, std::string const& msg) {
    add_error(kind, ast->token, msg);
  }
}