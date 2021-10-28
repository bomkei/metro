#pragma once

enum ErrorType
{
  ERR_ERROR,
  ERR_WARNING,
  ERR_NOTE
};

[[noreturn]] void fatal_error(std::string const& msg);

void error(ErrorType type, Token* tok, std::string const& msg);
void check_error();
