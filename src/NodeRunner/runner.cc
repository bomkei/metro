#include "Utils.h"
#include "NodeRunner.h"
#include "GC.h"

NodeRunner::NodeRunner() {
  GC::execute();
}

NodeRunner::~NodeRunner() {
  GC::stop();
}
