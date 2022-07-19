#pragma once

#include <memory>
#include "Types/BuiltinFunc.h"
#include "AppContext.h"

namespace Metro {
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