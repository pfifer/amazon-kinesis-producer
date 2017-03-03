//
// Created by Pfifer, Justin on 3/3/17.
//

#include "terminate_level_1.h"
#include "terminate_level.h"

namespace aws {
  namespace utils {
    void terminate_level_1::local_throw(int how) {
      LOCAL_THROW("level_1")
    }

    void terminate_level_1::level_2_throw(int how, bool in_catch, bool is_rethrow) {
      NEXT_THROW(level2, "level_2", local_throw(how), "level_1");
    }

    void terminate_level_1::level_3_throw(int how, bool in_catch, bool is_rethrow) {
      NEXT_THROW(level2, "level_2", level_3_throw(how, in_catch, is_rethrow), "level_1");
    }
  }
}
