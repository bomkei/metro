#pragma once

#include <string>
#include <vector>
#include <locale>
#include <codecvt>

#if METRO_DEBUG
  #define  alert  fprintf(stderr,"\t#alert " __FILE__ ":%d\n",__LINE__)
  #define  crash  { alert; fprintf(stderr,"\n\n# application has been crashed.\n"); exit(1); }
#else
  #define  alert  0
  #define  crash  exit(1);
#endif

#define  BIT(n)   (1 << n)
#define  TODO_IMPL  { alert; fprintf(stderr,"\n\n# Not implemented error.\n"); exit(1); }

namespace Utils {
  template <class... Args>
  std::string format(char const* fmt, Args&&... args) {
    static char buf[0x1000];
    sprintf(buf, fmt, args...);
    return buf;
  }

  namespace Strings {
    std::u16string to_u16string(std::string const& s);
    std::string to_string(std::u16string const& s);
  };
}
