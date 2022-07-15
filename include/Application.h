#pragma once

#include <memory>

namespace Metro {
  struct Object;
  class AppInternal;
  class Application {
  public:
    Application();
    ~Application();

    Object* execScriptFile(std::string const& path);

    int main(int argc, char** argv);

    static Application* get_instance();

  private:

    std::unique_ptr<AppInternal> _app;
  };
}