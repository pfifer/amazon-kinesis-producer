//
// Created by Pfifer, Justin on 3/3/17.
//

#include "terminate_level_2.h"

#include "terminate_level.h"

namespace aws {
  namespace utils {
    void terminate_level_2::local_throw(int how) {
      LOCAL_THROW("level_2");
    }

    void terminate_level_2::level_3_throw(int how, bool in_catch, bool is_rethrow) {
      NEXT_THROW(level3, "level_3", local_throw(how), "level_2");
    }
  }
}