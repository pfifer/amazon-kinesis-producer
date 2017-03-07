//
// Created by Pfifer, Justin on 3/7/17.
//

#ifndef AMAZON_KINESIS_PRODUCER_INTERNAL_AWSCLIENTEXECUTOR_H
#define AMAZON_KINESIS_PRODUCER_INTERNAL_AWSCLIENTEXECUTOR_H

#include <aws/core/utils/threading/Executor.h>

namespace aws {
  namespace utils {
    class AwsClientExecutor : public Aws::Utils::Threading::DefaultExecutor {

    protected:
      bool SubmitToThread(std::function<void()> &&function) override;

    };

  }
}


#endif //AMAZON_KINESIS_PRODUCER_INTERNAL_AWSCLIENTEXECUTOR_H
