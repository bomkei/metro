#include <memory>
#include "common.h"
#include "token.h"
#include "object.h"
#include "AST.h"
#include "error.h"
#include "utils.h"
#include "list_wrap.h"

#define cur_token ctx->token

static std::vector<std::pair<char const*, char const*>> const escape_sqeuence_pairs = {
  {"\\\\", "\\"}, {"\\n", "\n"}, {"\\t", "\t"}};

Node* expr();

namespace {
  struct ParserContext {
    Token* token;
    Token* csmtok;
  };

  std::shared_ptr<ParserContext> ctx;
  std::list<std::shared_ptr<ParserContext>> __list;

  void new_context() {
    __list.push_front(std::shared_ptr<ParserContext>(new ParserContext));
    ctx = *__list.begin();
  }

  void leave_context() {
    __list.pop_front();
  }

  Node* new_node(NodeKind kind = ND_VALUE) {
    auto nd = new Node;

    nd->kind  = kind;
    nd->lhs   = nullptr;
    nd->rhs   = nullptr;
    nd->token = cur_token;

    return nd;
  }

  Node* new_node(NodeKind kind, Node* lhs, Node* rhs, Token* tok = nullptr) {
    auto x = new_node(kind);

    x->lhs   = lhs;
    x->rhs   = rhs;
    x->token = tok ? tok : ctx->csmtok;

    return x;
  }

  Node* new_node_with_object(Object const& obj) {
    auto x = new_node();

    x->object = obj;

    return x;
  }

  bool check() {
    return cur_token->kind != TOK_EOF;
  }

  void next() {
    cur_token = cur_token->next;
  }

  bool consume(char const* str) {
    if (cur_token->str_check(str)) {
      ctx->csmtok = cur_token;
      next();
      return true;
    }

    return false;
  }

  void expect(char const* str, bool proceed = true) {
    if (proceed ? !consume(str) : !cur_token->str_check(str)) {
      error(ERR_ERROR, cur_token, Utils::format("expected '%s'", str));
      exit(1);
    }
  }

  void expect_ident() {
    if (cur_token->kind != TOK_IDENT)
      error(ERR_ERROR, cur_token, "expected identifier");
  }

  Node* expect_type() {
    auto type = new_node(ND_TYPE);

    type->type = new Node::Type;

    expect_ident();
    type->type->name = cur_token->getstr();

    next();

    if (consume("<")) {
      do {
        type->list.emplace_back(expect_type());
      } while (consume(","));

      if (cur_token->getstr() == ">>") {
        auto tk  = new Token;
        tk->kind = TOK_RESERVED;
        tk->str  = cur_token->str + 1;
        tk->len  = 1;

        cur_token->len = 1;
        cur_token->insert(tk);
      }

      expect(">");
    }

    while (consume("[")) {
      if (!consume("]")) {
        type->list.emplace_back(expr());
        expect("]");
      } else {
        type->list.emplace_back(nullptr);
      }
    }

    return type;
  }

  Node* check_semicolon(Node* node) {
    switch (node->kind) {
      case ND_FUNC:
      case ND_DATA:
      case ND_BLOCK:
      case ND_IF:
      case ND_FOR:
        break;

      default:
        expect(";");
    }

    return node;
  }

  void read_block(Node* node) {
    bool closed = false;

    while (check() && !(closed = consume("}"))) {
      node->list.emplace_back(check_semicolon(expr()));
    }

    if (!closed)
      error(ERR_ERROR, node->token, "not closed");
  }
} // namespace

Node* primary() {
  if (consume("{")) {
    auto node = new_node(ND_BLOCK);

    node->scope = new Node::Scope;

    node->token = ctx->csmtok;

    read_block(node);

    return node;
  }

  if (consume("[")) {
    auto node   = new_node(ND_ARRAY);
    node->token = ctx->csmtok;

    if (!consume("]")) {
      do {
        node->list.emplace_back(expr());
      } while (consume(","));
      expect("]");
    }

    return node;
  }

  if (consume("(")) {
    auto x = expr();
    expect(")");
    return x;
  }

  if (consume("if")) {
    auto node   = new_node(ND_IF);
    node->token = ctx->csmtok;

    node->if_stmt = new Node::If;

    node->if_stmt->cond = expr();

    expect("{", false);
    node->if_stmt->true_block = expr();

    if (consume("else")) {
      if (cur_token->getstr() != "if")
        expect("{", false);

      node->if_stmt->false_block = expr();
    }

    return node;
  }

  if (consume("for")) {
    auto node      = new_node(ND_FOR);
    node->for_stmt = new Node::For;

    node->for_stmt->begin = expr();
    expect(";");

    node->for_stmt->cond = expr();
    expect(";");

    node->for_stmt->count = expr();

    expect("{", false);
    node->for_stmt->code = expr();

    if (consume("else")) {
      node->for_stmt->else_code = expr();
    }

    return node;
  }

  if (consume("true")) {
    auto node = new_node(ND_VALUE);

    node->object.type.kind = OBJ_BOOL;
    node->object.v_bool    = true;

    return node;
  }

  if (consume("false")) {
    auto node = new_node(ND_VALUE);

    node->object.type.kind = OBJ_BOOL;
    node->object.v_bool    = false;

    return node;
  }

  if (consume("none")) {
    return new_node(ND_VALUE);
  }

  if (consume("new")) {
    auto node     = new_node(ND_NEW);
    node->newdata = new Node::NewData;
    node->token   = ctx->csmtok;

    if (cur_token->kind == TOK_IDENT) {
      node->newdata->name = cur_token->getstr();
      next();
    }

    expect("{");

    if (!consume("}")) {
      do {
        auto init = new Node::Initializer;

        expect_ident();
        init->name  = cur_token->getstr();
        init->token = cur_token;

        next();
        expect(":");
        init->expr = expr();

        node->newdata->initializer_list.emplace_back(init);
      } while (consume(","));
      expect("}");
    }

    return node;
  }

  switch (cur_token->kind) {
    case TOK_INT:
    case TOK_HEX: {
      auto x = new_node();

      x->object.type.kind = OBJ_INT;
      x->object.v_int     = cur_token->kind == TOK_INT ? atoi(cur_token->str) : strtol(cur_token->str, nullptr, 16);

      next();
      return x;
    }

    case TOK_CHAR: {
      auto x = new_node();

      x->object.type.kind = OBJ_CHAR;
      x->object.v_char    = Utils::Strings::to_u16string(cur_token->getstr())[0];

      next();
      return x;
    }

    case TOK_DOUBLE: {
      auto x = new_node();

      x->object.type.kind = OBJ_DOUBLE;
      x->object.v_double  = strtod(cur_token->str, nullptr);

      next();
      return x;
    }

    case TOK_STRING: {
      auto x = new_node();

      x->object.type.kind = OBJ_STRING;

      auto&& str = cur_token->getstr();

      for (i64 i = 0; i < str.length();) {
        for (auto&& esc : escape_sqeuence_pairs) {
          std::string&& cmp = std::get<0>(esc);
          auto& repl        = std::get<1>(esc);

          if (i + cmp.length() <= str.length() && str.substr(i, cmp.length()) == cmp) {
            str.erase(i, cmp.length());
            str.insert(i, repl);
            i += strlen(repl);
            goto cnt_loop1;
          }
        }

        i++;
      cnt_loop1:;
      }

      x->object.v_str = Utils::Strings::to_u16string(str);

      next();
      return x;
    }

    case TOK_IDENT: {
      auto x = new_node(ND_VARIABLE);
      next();

      if (consume("(")) {
        x->kind     = ND_CALLFUNC;
        x->callfunc = new Node::CallFunc;

        if (!consume(")")) {
          do {
            x->list.emplace_back(expr());
          } while (consume(","));
          expect(")");
        }
      }

      return x;
    }
  }

  error(ERR_ERROR, cur_token, "syntax error");
  exit(1);
}

Node* indexref() {
  auto x = primary();

  while (consume("[")) {
    x = new_node(ND_INDEXREF, x, expr());
    expect("]");
  }

  return x;
}

Node* memberaccess() {
  auto x = indexref();

  while (consume(".")) {
    auto tok = ctx->csmtok;
    auto y   = indexref();

    if (y->kind == ND_CALLFUNC) {
      y->list.insert(y->list.begin(), x);
      x = y;
    } else {
      if (y->kind != ND_VARIABLE)
        error(ERR_ERROR, tok, "syntax error");

      x = new_node(ND_MEMBER_ACCESS, x, y);
    }
  }

  return x;
}

Node* unary() {
  if (consume("-")) {
    return new_node(ND_SUB, new_node_with_object(OBJ_INT), memberaccess());
  }

  return memberaccess();
}

Node* mul() {
  auto x = unary();
  while (check()) {
    if (consume("*"))
      x = new_node(ND_MUL, x, unary());
    else if (consume("/"))
      x = new_node(ND_DIV, x, unary());
    else if (consume("%"))
      x = new_node(ND_MOD, x, unary());
    else
      break;
  }
  return x;
}

Node* add() {
  auto x = mul();
  while (check()) {
    if (consume("+"))
      x = new_node(ND_ADD, x, mul());
    else if (consume("-"))
      x = new_node(ND_SUB, x, mul());
    else
      break;
  }
  return x;
}

Node* shift() {
  auto x = add();
  while (check()) {
    if (consume("<<"))
      x = new_node(ND_LSHIFT, x, add());
    else if (consume(">>"))
      x = new_node(ND_RSHIFT, x, add());
    else
      break;
  }
  return x;
}

Node* compare() {
  auto x = shift();
  while (check()) {
    if (consume(">"))
      x = new_node(ND_BIGGER, x, shift());
    else if (consume("<"))
      x = new_node(ND_BIGGER, shift(), x);
    else if (consume(">="))
      x = new_node(ND_BIG_OR_EQ, x, shift());
    else if (consume("<="))
      x = new_node(ND_BIG_OR_EQ, shift(), x);
    else if (consume("<=>"))
      x = new_node(ND_SPACESHIP, shift(), x);
    else if (consume("=="))
      x = new_node(ND_EQUAL, shift(), x);
    else if (consume("!="))
      x = new_node(ND_NOT_EQUAL, shift(), x);
    else
      break;
  }
  return x;
}

Node* bit() {
  auto x = compare();

  while (check()) {
    if (consume("&"))
      x = new_node(ND_BIT_AND, x, compare());
    else if (consume("|"))
      x = new_node(ND_BIT_OR, x, compare());
    else if (consume("^"))
      x = new_node(ND_BIT_XOR, x, compare());
    else
      break;
  }

  return x;
}

Node* log_and() {
  auto x = bit();

  while (check()) {
    if (consume("&&"))
      x = new_node(ND_LOG_AND, x, bit());
    else if (consume("||"))
      x = new_node(ND_LOG_OR, x, bit());
    else
      break;
  }

  return x;
}

Node* assign() {
  auto x = log_and();

  if (consume("="))
    x = new_node(ND_ASSIGN, x, assign());
  else if (consume("+="))
    x = new_node(ND_ASSIGN, x, new_node(ND_ADD, x, assign()));
  else if (consume("-="))
    x = new_node(ND_ASSIGN, x, new_node(ND_SUB, x, assign()));
  else if (consume("*="))
    x = new_node(ND_ASSIGN, x, new_node(ND_MUL, x, assign()));
  else if (consume("/="))
    x = new_node(ND_ASSIGN, x, new_node(ND_DIV, x, assign()));
  else if (consume("<<="))
    x = new_node(ND_ASSIGN, x, new_node(ND_LSHIFT, x, assign()));
  else if (consume(">>="))
    x = new_node(ND_ASSIGN, x, new_node(ND_RSHIFT, x, assign()));

  return x;
}

Node* expr() {
  if (consume("let")) {
    auto node   = new_node(ND_LET);
    node->token = ctx->csmtok;

    node->declvar = new Node::DeclVar;

    expect_ident();
    node->declvar->name = cur_token->getstr();

    next();

    if (consume(":")) {
      node->declvar->type = expect_type();
    } else {
      node->declvar->type = nullptr;
    }

    if (consume("=")) {
      node->declvar->init_expr = expr();
    } else {
      node->declvar->init_expr = nullptr;
    }

    return node;
  }

  return assign();
}

Node* func() {
  if (consume("fn")) {
    auto node = new_node(ND_FUNC);

    node->func  = new Node::Function;
    node->scope = new Node::Scope;

    node->func->type = nullptr;

    expect_ident();
    node->func->name = cur_token->getstr();

    next();
    expect("(");

    if (!consume(")")) {
      do {
        auto arg = new Node::Argument;

        arg->type = expect_type();

        expect_ident();
        arg->token = cur_token;
        arg->name  = cur_token->getstr();

        next();
        node->func->args.emplace_back(arg);
      } while (consume(","));
      expect(")");
    }

    //
    // tag
    if (consume("for")) {
      expect_ident();

      node->func->data_tag = cur_token->getstr();

      next();
    }

    if (consume("->")) {
      node->func->type = expect_type();
    }

    expect("{", false);
    node->func->code = expr();

    return node;
  }

  if (consume("data")) {
    auto node  = new_node(ND_DATA);
    node->data = new Node::Data;

    expect_ident();
    node->data->name = cur_token->getstr();

    next();
    expect("{");

    if (!consume("}")) {
      do {
        auto member = new Node::Argument;

        if (consume("}")) {
          error(ERR_ERROR, ctx->csmtok, "expected identifier");
          error(ERR_ERROR, ctx->csmtok->back, "maybe you forgot remove this comma?");
          exit(1); // prevent too many error occur
        }

        expect_ident();
        member->name  = cur_token->getstr();
        member->token = cur_token;

        next();
        expect(":");
        member->type = expect_type();

        node->data->member_list.emplace_back(member);
      } while (consume(","));
      expect("}");
    }

    return node;
  }

  return expr();
}

Node* parse(Token* token) {
  new_context();

  ctx->token  = token;
  ctx->csmtok = nullptr;

  auto node   = new_node(ND_BLOCK);
  node->scope = new Node::Scope;

  while (check()) {
    node->list.emplace_back(check_semicolon(func()));
  }

  leave_context();
  return node;
}