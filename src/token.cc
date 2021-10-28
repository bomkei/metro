#include "common.h"
#include "token.h"

bool Token::str_check(char const* str) const {
  if (!this->str)
    return false;

  u32 slen = strlen(str);
  return this->len == slen && memcmp(this->str, str, slen) == 0;
}

void Token::insert(Token* token) {
  token->next = this->next;
  this->next  = token;
}

std::string Token::getstr() const {
  return std::string(this->str, this->len);
}