#include <fstream>
#include "Application.h"

int main(int argc, char** argv) {
  auto app = new Metro::Application();

  app->initialize();
  app->parse_args(argc, argv);

  return app->main(argc, argv);
}
