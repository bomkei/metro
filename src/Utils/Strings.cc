#include "Utils.h"

namespace Utils::Strings {
  static std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
  
  std::u16string to_u16string(std::string const& s) {
    return conv.from_bytes(s);
  }

  std::string to_string(std::u16string const& s) {
    return conv.to_bytes(s);
  }
}