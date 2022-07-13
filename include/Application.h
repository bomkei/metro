#pragma once

class Application {
public:

  Application(bool const);

  int main(int argc, char** argv);

  bool is_debug_enabled();

  static Application* get_instance();

private:

  bool const _debug;
};
