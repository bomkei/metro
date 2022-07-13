#pragma once

#include <string>
#include <vector>

//#define  METRO_DEBUG  1
#define  METRO_DEBUG  (Application::get_instance()->is_debug_enabled())

#define  BIT(n)   (1 << n)

#define  alert  _Alert(__FILE__,__LINE__)
#define  crash  _Crash(__FILE__,__LINE__)

// debugs
// inline static int _Alert(char const* f, int n) {
//   return METRO_DEBUG ? fprintf(stderr,"\t#alert %s:%d\n",f,n) : 0;
// }

// inline static void _Crash(char const* f, int n) {
//   fprintf(stderr,"crashed!! %s:%d\n",f,n);
//   exit(1);
// }

int _Alert(char const* f, int n);
void _Crash(char const* f, int n);

namespace Utils {
  template <class... Args>
  std::string format(char const* fmt, Args&&... args) {
    static char buf[0x1000];
    sprintf(buf, fmt, args...);
    return buf;
  }

  class Strings {
    static inline std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
  
  public:
    static auto to_wstring(std::string const& s) {
      return conv.from_bytes(s);
    }

    static auto to_string(std::u16string const& s) {
      return conv.to_bytes(s);
    }
  };
}
