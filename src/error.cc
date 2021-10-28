#include "common.h"
#include "token.h"
#include "driver.h"
#include "utils.h"
#include "error.h"

#define ERROR_EMIT_MAX 20

extern Driver driver;

namespace {
  u32 error_emitted_count;
}

void fatal_error(std::string const& msg) {
  std::cout << COL_RED << "fatal error: " << COL_DEFAULT << msg << std::endl;
  exit(1);
}

void error(ErrorType type, Token* tok, std::string const& msg) {
  std::string const& src = driver.source_code;
  u32 errpos             = tok->pos;

  u32 line_begin = 0;
  u32 line_end   = src.length();
  u32 line_num   = 1;

  for (u32 i = 0; i < errpos; i++) {
    if (src[i] == '\n') {
      line_num++;
      line_begin = i + 1;
    }
  }

  for (u32 i = errpos; i < line_end; i++) {
    if (src[i] == '\n') {
      line_end = i;
      break;
    }
  }

  char const* color      = "";
  char const* errtypestr = "";

  switch (type) {
    case ERR_ERROR:
      color = COL_RED;
      break;
    case ERR_WARNING:
      color      = COL_PINK;
      errtypestr = "warning:";
      break;
    case ERR_NOTE:
      color      = COL_SKYBLUE;
      errtypestr = "note:";
      (void)errtypestr;
      break;
  }

  printf(
    COL_SKYBLUE TXT_BOLD "      --> " TXT_DEFAULT COL_GREEN "%s:%u\n", driver.get_cur_script_path().c_str(), line_num);

  printf(COL_SKYBLUE "%6u" TXT_BOLD " | " COL_WHITE TXT_DEFAULT "%s\n", line_num,
    src.substr(line_begin, line_end - line_begin).c_str());

  printf(COL_SKYBLUE TXT_BOLD "       | %s%s%s %s\n\n" TXT_DEFAULT, std::string(errpos - line_begin, ' ').c_str(),
    color, std::string(tok->len, '^').c_str(), msg.c_str());

  if (type == ERR_ERROR)
    error_emitted_count += 1;

  if (error_emitted_count >= ERROR_EMIT_MAX) {
    printf(COL_RED "\napplication has been killed due to too many error "
                   "occurred\n" COL_DEFAULT);
    exit(1);
  }
}

void check_error() {
  if (error_emitted_count != 0) {
    exit(1);
  }
}