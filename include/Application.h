#pragma once

#include <list>
#include <memory>
#include "Types/BuiltinFunc.h"
#include "AppContext.h"

namespace Metro {
  class Application {
  public:
    Application();
    ~Application();

    void initialize();

    AppContext::Script open_script_file(char const* path);
    Object* execute_script(AppContext::Script& script);

    bool parse_args(int argc, char** argv);
    void show_help();
    void show_version();

    int main(int argc, char** argv);

    static Application* get_instance();
    static AppContext const* get_cur_appcontext();
    static AppContext::Script const* get_running_script();

  private:
    AppContext ctx;

    static std::list<AppContext::Script*> running_script;
  };
}