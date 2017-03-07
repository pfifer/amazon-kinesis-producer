//
// Created by Pfifer, Justin on 3/7/17.
//

#ifndef AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_H
#define AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_H

#include <thread>
#include <functional>
#include <aws/mutex.h>

namespace aws {
  namespace utils {
    aws::thread make_reporting_thread(std::function<void()> &&thread_function);
  }
}


#endif //AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_H
