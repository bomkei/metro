#pragma once

#include <cassert>

#define __DEBUG__ 1

#define TXT_DEFAULT "\e[0m"
#define TXT_BOLD "\e[1m"

#define COL_DEFAULT "\033[37m"
#define COL_WHITE "\033[97m"
#define COL_SKYBLUE "\033[96m"
#define COL_PINK "\033[95m"
#define COL_BLUE "\033[94m"
#define COL_YELLOW "\033[93m"
#define COL_GREEN "\033[92m"
#define COL_RED "\033[91m"
#define COL_GRAY "\033[90m"
#define COL_CYAN "\033[36m"
#define COL_MAGENTA "\033[35m"

#define BIT(n) (1 << n)

#if __DEBUG__
#define alert fprintf(stderr, "\t#alert at %s:%d\n", __FILE__, __LINE__)
#define crash                                                                                                          \
  fprintf(stderr, "\ncrashed at %s:%d\n", __FILE__, __LINE__);                                                         \
  (void)(*(int*)0 = 0)
#define debug(...) __VA_ARGS__
#define dbgprint(...)                                                                                                  \
  {                                                                                                                    \
    fprintf(stderr, __VA_ARGS__);                                                                                      \
    fflush(stderr);                                                                                                    \
  }
#define loop(n, ...)                                                                                                   \
  for (int i = 0; i < n; i++) {                                                                                        \
    __VA_ARGS__                                                                                                        \
  }
#define print_line(...)                                                                                                \
  {                                                                                                                    \
    fprintf(stderr, "\n------------------\n%s\tline %6d| %s\n\n", __FILE__, __LINE__, #__VA_ARGS__);                   \
    __VA_ARGS__                                                                                                        \
  }
#else
#define alert 0
#define crash 0
#define debug(...) 0;
#define dbgprint(...) 0
#define loop(...) 0
#define print_line(...) __VA_ARGS__
#endif

#define TODO_IMPL                                                                                                      \
  alert;                                                                                                               \
  fprintf(stderr, "Not implemented error.\n");                                                                         \
  exit(1)

#define OOPS(code)                                                                                                     \
  fprintf(stderr,                                                                                                      \
    "\n\n"                                                                                                             \
    "Internal error have been occurred. Please report error code to developer.\n"                                      \
    "\nerror code: %08X\n",                                                                                            \
    (u32)code);                                                                                                        \
  exit(1)
