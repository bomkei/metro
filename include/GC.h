#pragma once

#include "Types/Object.h"

class GC {
public:
  static void execute();
  static void stop();

  static Object* append(Object* obj);
};

template <class... Args>
inline Object* gcnew(Args&&... args) {
  return GC::append(new Object(args...));
}
