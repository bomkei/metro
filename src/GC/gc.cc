#include <thread>
#include <mutex>
#include <map>
#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "GC.h"
#include "Utils.h"

namespace Metro {
  namespace {
    bool is_running;
    std::thread* thread;
    std::vector<Object*> objects;
    std::map<Object*, clock_t> objclock;
    std::mutex mtx;

    // delete unused object
    void clean() {
      std::lock_guard<std::mutex> lock{ mtx };

      // 1000000 = 1sec

      for( size_t i = 0; i < objects.size(); ) {
        auto& obj = objects[i];

        if( (clock() - objclock[obj]) >= 1000000 ) {
          if( obj->ref_count == 0 ) {
            delete obj;
            objects.erase(objects.begin() + i);
          }
        }
        else {
          i++;
        }
      }
    }

    void thread_routine() {
      while( is_running ) {
        clean();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
    }
  }

  void GC::execute() {
    is_running = true;
    thread = new std::thread(thread_routine);

    alert;
  }

  void GC::stop() {
    is_running = false;
    thread->detach();

    for( auto&& obj : objects ) {
      delete obj;
    }
  }

  Object* GC::append(Object* obj) {
    std::lock_guard<std::mutex> lock{ mtx };

    objclock[obj] = clock();
    return objects.emplace_back(obj);
  }
}