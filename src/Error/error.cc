#include <iostream>
#include "Application.h"
#include "Error.h"
#include "Utils.h"

namespace Metro {
  void ErrorContext::show() {
    alert;

    auto spacec = err_pos - err_begin;

    std::cerr
      << std::string_view(script->data.data() + err_begin, err_end - err_begin) << std::endl
      << std::string(spacec, ' ') << std::string(err_underline_length, '^') << ' ' << message << std::endl;
  }

  void Error::show_all() {
    for( auto&& ctx : contexts ) {
      ctx.show();
    }
  }

  void Error::check() {
    if( !contexts.empty() ) {
      exit_app();
    }
  }

  void Error::exit_app() {
    show_all();
    exit(1);
  }
}