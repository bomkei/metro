#include <fstream>
#include "Application.h"

int main(int argc, char** argv) {
  auto app = new Metro::Application();

  app->initialize();
  app->execScriptFile("test.txt");

  return 0;
}
