#pragma once

#include <locale>
#include <codecvt>

namespace Utils {
  class Strings {
  public:
    static inline std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;

    static std::u16string to_u16string(std::string const& str) {
      return conv.from_bytes(str);
    }

    static std::string to_string(std::u16string const& str) {
      return conv.to_bytes(str);
    }

    static void replace_destructive(std::string& str, std::string_view find, std::string_view replace) {
      for (i64 i = 0; i < (signed)str.length() - find.length();) {
        if (str.substr(i, find.length()) == find) {
          str.erase(i, find.length());
          str.insert(i, replace);
          i += replace.length();
        } else {
          i++;
        }
      }
    }

    static std::string replace(std::string str, std::string_view find, std::string_view replace) {
      replace_destructive(str, find, replace);
      return str;
    }
  };

  template <class... Args>
  std::string format(char const* fmt, Args&&... args) {
    static char buf[0x1000];
    sprintf(buf, fmt, args...);
    return buf;
  }
} // namespace Utils

template <class T>
concept HaveBeginAndEnd = requires(T const& x) {
  x.begin() != x.end();
};

template <class T>
concept HaveToString = requires(T const& x) {
  x.to_string();
};

template <class T>
concept ConvertibleToString = requires(T const& x) {
  std::to_string(x);
};

//
/// operator << wrapper
template <HaveToString T>
inline auto& operator<<(auto& stream, T const& obj) {
  return stream << obj.to_string();
}

template <HaveToString T>
inline std::string to_string_wrap(T const& x) {
  return x.to_string();
}

template <ConvertibleToString T>
inline std::string to_string_wrap(T const& x) {
  return std::to_string(x);
}

inline auto to_string_wrap(std::string const& x) {
  return x;
}

template <class T>
inline auto join(std::string const& sep, std::vector<T> const& vec) {
  std::string str;

  for (std::size_t i = 0; i < vec.size(); i++) {
    str += to_string_wrap(vec[i]);
    if (i < vec.size() - 1)
      str += sep;
  }

  return str;
}

template <class T, class Func>
inline auto join_with_method(std::string const& sep, std::vector<T> const& vec, Func to_str_method) {
  std::string str;

  for (std::size_t i = 0; i < vec.size(); i++) {
    str += to_str_method(vec[i]);
    if (i < vec.size() - 1)
      str += sep;
  }

  return str;
}

template <class T, class F>
std::vector<T> take_out_from_vector(auto&& vec, F fun) {
  std::vector<T> v;

  for (auto&& i : vec)
    v.emplace_back(fun(i));

  return v;
}

inline bool startswith(std::string const& str, std::string const& str2) {
  return str.find(str2) == 0;
}

template <class INT>
inline void print_bin(INT num) {
  u32 ax = sizeof(INT) << 2;

  for (i32 i = ax - 1; i >= 0; i--) {
    printf("%d", (num >> i) & 1);
  }

  puts("");
}
