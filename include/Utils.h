#pragma once

#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include "Debug.h"

#define  BIT(n)   (1 << n)

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
