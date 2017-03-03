//
// Created by Pfifer, Justin on 3/3/17.
//

#include "terminate_level_2.h"

#ifndef AMAZON_KINESIS_PRODUCER_INTERNAL_TERMINATE_LEVEL_1_H
#define AMAZON_KINESIS_PRODUCER_INTERNAL_TERMINATE_LEVEL_1_H

namespace aws {
  namespace utils {
    class terminate_level_1 {

    private:
      terminate_level_2 level2;

    public:
      void local_throw(int how);
      void level_2_throw(int how, bool in_catch, bool is_rethrow);
      void level_3_throw(int how, bool in_catch, bool is_rethrow);
    };
  }
}



#endif //AMAZON_KINESIS_PRODUCER_INTERNAL_TERMINATE_LEVEL_1_H
