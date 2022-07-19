#include <vector>
#include "Types/Token.h"
#include "Types/Node.h"
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
    
  }

  void Error::add_error(ErrorKind kind, Node* node, std::string const& msg) {
    auto& ctx = contexts.emplace_back();

    TODO_IMPL

  }
}