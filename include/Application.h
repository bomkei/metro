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
    std::vector<Node*> construct_sysmodule();

    AppContext::Script open_script_file(char const* path);
    Object* execute_script(AppContext::Script& script);

    bool parse_args(int argc, char** argv);
    void show_help();
    void show_version();

    int main(int argc, char** argv);

    static Application* get_instance();
    static AppContext const* get_cur_appcontext();

    static bool wasSysmoduleConstructed();
    static std::vector<Node*> const& get_sysmodules();

  private:
    AppContext ctx;
    static std::vector<Node*> sysmodules;
  };
}