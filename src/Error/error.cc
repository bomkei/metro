#include <iostream>
#include "Application.h"
#include "Error.h"
#include "Utils.h"

namespace Metro {
  void ErrorContext::show() {

    alert;
    std::cout << Utils::format("ErrorContext: err_pos = %zu", err_pos) << std::endl;

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