#ifndef AMAZON_KINESIS_PRODUCER_THREAD_POOL_SELECTOR_H
#define AMAZON_KINESIS_PRODUCER_THREAD_POOL_SELECTOR_H

#include <memory>
#include "reporting_thread_executor.h"
#include "simple_executor.h"

#include <aws/core/utils/threading/Executor.h>

namespace aws {
  namespace utils {

    std::shared_ptr<Aws::Utils::Threading::Executor> make_executor();

  }
}



#endif //AMAZON_KINESIS_PRODUCER_THREAD_POOL_SELECTOR_H
