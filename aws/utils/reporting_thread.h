//
// Created by Pfifer, Justin on 3/7/17.
//

#include <thread>
#include <functional>

#ifndef AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_H
#define AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_H

namespace aws {
  namespace utils {
    std::thread &&make_thread(std::function<void()> &&thread_function);
  }
}


#endif //AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_H
