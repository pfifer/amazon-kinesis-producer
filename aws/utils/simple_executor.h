//
// Created by Pfifer, Justin on 4/24/17.
//

#ifndef AMAZON_KINESIS_PRODUCER_SIMPLE_EXECUTOR_H
#define AMAZON_KINESIS_PRODUCER_SIMPLE_EXECUTOR_H


#include <aws/core/utils/threading/Executor.h>
#include "reporting_thread.h"

namespace aws {
  namespace utils {
    class simple_executor : public Aws::Utils::Threading::DefaultExecutor {

    public:
      simple_executor() {}

    protected:
      bool SubmitToThread(std::function<void()> &&function) override;
    };
  }
}



#endif //AMAZON_KINESIS_PRODUCER_SIMPLE_EXECUTOR_H
