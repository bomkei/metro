#include <memory>
#include "common.h"
#include "token.h"
#include "error.h"

static char const* operator_token_list[] = {"...", ">>=", "<<=", "<=>", "->", "<-", ">=", "<=", ">>", "<<",
  "==", "!=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "[]", "&&", "||", "::", "!", "=", ".", ",", "?", "~", ">", "<",
  "%", "&", "^", "|", "(", ")", "[", "]", "{", "}", ";", ":", "+", "-", "*", "/", "@", "\n"};

namespace {
  struct TokenizerCtx {
    char const* source;
    u32 position;
    u32 const length;

    explicit TokenizerCtx(char const* src)
        : source(src)
        , position(0)
        , length(strlen(src)) {
    }
  };

  std::shared_ptr<TokenizerCtx> ctx;
  std::list<std::shared_ptr<TokenizerCtx>> __list;

  void new_context(char const* src) {
    __list.push_front(std::shared_ptr<TokenizerCtx>(new TokenizerCtx(src)));
    ctx = *__list.begin();
  }

  void leave_context() {
    __list.pop_front();
  }

  bool check() {
    return ctx->position < ctx->length;
  }

  char peek() {
    return ctx->source[ctx->position];
  }

  void next() {
    ctx->position += 1;
  }

  bool match(char const* str, u32 len) {
    return ctx->position + len <= ctx->length && memcmp(ctx->source + ctx->position, str, len) == 0;
  }

  void pass_space() {
    while (check() && peek() <= ' ') {
      next();
    }
  }

  int pass_num() {
    int i = 0;

    while (check() && isdigit(peek())) {
      i += 1, next();
    }

    return i;
  }
} // namespace

Token* new_token(TokenKind kind, Token* back, u32 pos) {
  Token* tok = (Token*)calloc(1, sizeof(Token));

  tok->kind = kind;
  tok->back = back;
  tok->pos  = pos;

  if (back)
    back->next = tok;

  return tok;
}

Token* tokenize(char const* src) {
  Token top;
  Token* cur = &top;

  new_context(src);
  pass_space();

  while (check()) {
    char ch = peek();
    u32 pos = ctx->position;

    cur      = new_token(TOK_INT, cur, pos);
    cur->str = ctx->source + pos;

    if (match("0x", 2)) {
      while (check() && isalnum(peek()))
        cur->len += 1, ctx->position += 1;
    }

    else if (isdigit(ch)) {
      cur->len = pass_num();

      if (peek() == '.') {
        ctx->position += 1;

        if (!isdigit(peek())) {
          ctx->position -= 1;
          continue;
        }

        cur->kind = TOK_DOUBLE;
        cur->len += pass_num() + 1;
      }
    }

    else if (isalpha(ch) || ch == '_') {
      cur->kind = TOK_IDENT;

      while (check() && (isalnum(ch = peek()) || ch == '_')) {
        ctx->position++, cur->len++;
      }
    }

    else if (ch == '"') {
      cur->kind = TOK_STRING;
      cur->str++;

      for (ctx->position++; peek() != '"'; ctx->position++)
        cur->len++;

      ctx->position++;
    }

    else if (ch == '\'') {
      cur->kind = TOK_CHAR;

      ctx->position++;
      cur->str++;
      cur->len = 1;

      ctx->position++;
      if (peek() != '"')
        error(ERR_ERROR, cur, "not closed");

      ctx->position++;
    }

    else {
      for (auto&& str : operator_token_list) {
        u32 len = strlen(str);

        if (match(str, len)) {
          cur->kind = TOK_RESERVED;
          cur->str  = str;
          cur->len  = len;
          ctx->position += len;
          goto done;
        }
      }

      error(ERR_ERROR, cur, "unknown token");
      exit(1);
    done:;
    }

    pass_space();
  }

  new_token(TOK_EOF, cur, ctx->position)->str = "";
  return top.next;
}