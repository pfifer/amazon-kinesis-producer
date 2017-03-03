//
// Created by Pfifer, Justin on 3/3/17.
//

#include "terminate_level_3.h"

#include "terminate_level.h"

namespace aws {
  namespace utils {
    void terminate_level_3::local_throw(int how) {
      LOCAL_THROW("level_3");
    }
  }
}