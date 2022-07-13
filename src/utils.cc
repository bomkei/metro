#include "Application.h"
#include "Utils.h"

int _Alert(char const* f, int n) {
  return METRO_DEBUG ? fprintf(stderr,"\t#alert %s:%d\n",f,n) : 0;
}

void _Crash(char const* f, int n) {
  fprintf(stderr,"crashed!! %s:%d\n",f,n);
  exit(1);
}

namespace Utils::Strings {
  static std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
  
  std::u16string to_u16string(std::string const& s) {
    return conv.from_bytes(s);
  }

  std::string to_string(std::u16string const& s) {
    return conv.to_bytes(s);
  }
}