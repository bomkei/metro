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

    // --- Debugs --- //
  #if METRO_DEBUG
    // how far run app
    //  0. Nothing
    //  1. Lex
    //  2. Parse
    //  3. Semantics
    //  4. Evaluate
    int     _d_max_step_to = 0;

    // flags
    struct __attribute__((packed)) _dt_ctrl_print_flags {
      bool _df_tokens        : 1; // q
      bool _df_nodes         : 1; // w
      bool _df_sema_result   : 1; // e
      bool _df_evaluated_obj : 1; // r
      bool _df_construct_obj : 1; // t
      bool _df_destruct_obj  : 1; // y
    };

    union {
      struct {
        _dt_ctrl_print_flags    _d_print;
      };

      uint32_t __dbg_all_flag_bits = 0;
    };

  #endif
    // -------------- //

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
    static AppContext const* get_cur_appcontext();

    static bool wasSysmoduleConstructed();
    static Node* get_sysmodules_node();

  private:

    AppContext ctx;
    static Node* sysmodules_node;
  };
}