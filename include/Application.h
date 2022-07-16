#pragma once

#include <memory>
#include "Types/BuiltinFunc.h"

namespace Metro {
  struct Node;
  struct Object;

  struct AppContext {
    struct Script {
      char const*     path;
      std::string     data;

      Node*   node = nullptr;
    };

    int     argc;
    char**  argv;
    std::string_view  app_name;

    // --- Flags --- //
    bool    is_debug_enabled      = 0;
    bool    no_print_filenames    = 0; // don't print script path while running multiple script files

    std::vector<Script>   scripts;
  };

  class Application {
  public:

    Application();
    ~Application();

    void initialize();
    Node* construct_sysmodule();

    AppContext::Script open_script_file(char const* path);
    Object* execute_script(AppContext::Script& script);

    bool parse_args(int argc, char** argv);
    void show_help();
    void show_version();

    int main(int argc, char** argv);

    static Application* get_instance();
    static bool wasSysmoduleConstructed();
    static Node* get_sysmodules_node();

  private:

    AppContext ctx;
    static Node* sysmodules_node;
  };
}