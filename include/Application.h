#pragma once

class Application {
public:

  Application();

  int main(int argc, char** argv);

  static Application* get_instance();

private:
};
