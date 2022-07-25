#pragma once

#include <string>
#include <vector>

namespace Metro {
  namespace AST {
    struct Base;
  }

  struct AppContext {
    struct Script {
      char const*     path;
      std::string     data;

      Token*          token;
      AST::Base*      ast;

      Script()
        : path(nullptr),
          token(nullptr),
          ast(nullptr)
      {
      }
    };

    int     argc;
    char**  argv;
    std::string_view  app_name;

    // --- Flags --- //
    bool    no_print_filenames; // don't print script path while running multiple script files

    std::vector<Script>   scripts;

    AppContext()
      : argc(0),
        argv(nullptr),
        no_print_filenames(false)
    {
    }
  };
}