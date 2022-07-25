#include "Utils.h"
#include "Evaluator.h"
#include "GC.h"

namespace Metro {
  Evaluator::Evaluator() {
    GC::execute();
  }

  Evaluator::~Evaluator() {
    GC::stop();
  }
}
