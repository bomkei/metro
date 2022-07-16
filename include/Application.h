#pragma once

#include <memory>
#include "BuiltinFunc.h"

namespace Metro {
  struct Node;
  struct Object;
  class Application {
  public:

    Application();
    ~Application();

    void initialize();
    Node* construct_sysmodule();

    Object* execScriptFile(std::string const& path);

    int main(int argc, char** argv);

    static Application* get_instance();
    static bool wasSysmoduleConstructed();

  private:

    static Node* sysmodules_node;
  };
}