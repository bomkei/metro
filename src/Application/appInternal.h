#pragma once

namespace Metro {
  class AppInternal {
    friend class Application;

    bool isConstructedSystemModules;
  };
}