//
// Created by Pfifer, Justin on 3/7/17.
//

#ifndef AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_EXECUTOR_H
#define AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_EXECUTOR_H

#include <aws/core/utils/threading/Executor.h>
#include "reporting_thread.h"

namespace aws {
  namespace utils {
    class reporting_thread_executor : public Aws::Utils::Threading::DefaultExecutor {

    protected:
      bool SubmitToThread(std::function<void()> &&function) override;
    };
  }
}



#endif //AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_EXECUTOR_H
