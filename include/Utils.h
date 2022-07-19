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

  template <class T, class _E = std::remove_all_extents_t<T>, class F = std::string(*)(_E)>
  std::string join(std::string s, std::vector<T> const& vec, F conv = std::to_string) {
    std::string ret;

    for( auto it = vec.begin(); it != vec.end(); it++ ) {
      if constexpr( !std::is_same_v<_E, char const*> && !std::is_same_v<_E, std::string> ) {
        ret += conv(*it);
      }
      else {
        ret += *it;
      }

      if( it < vec.end() - 1 ) ret += s;
    }
    
    return ret;
  }

  namespace Strings {
    std::u16string to_u16string(std::string const& s);
    std::string to_string(std::u16string const& s);
  }
}
