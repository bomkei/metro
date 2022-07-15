#pragma once

namespace Metro {
  class AppInternal {
    friend class Application;

    bool isConstructedSystemModulesNode;

    Node* construct_sysmodules_node();

  public:
    ~AppInternal() = default;
  };
}