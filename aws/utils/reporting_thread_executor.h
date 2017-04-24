// Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Amazon Software License (the "License").
// You may not use this file except in compliance with the License.
// A copy of the License is located at
//
//  http://aws.amazon.com/asl
//
// or in the "license" file accompanying this file. This file is distributed
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied. See the License for the specific language governing
// permissions and limitations under the License.

#ifndef AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_EXECUTOR_H
#define AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_EXECUTOR_H

#include <aws/core/utils/threading/Executor.h>
#include "reporting_thread.h"

namespace aws {
  namespace utils {
    class reporting_thread_executor : public Aws::Utils::Threading::PooledThreadExecutor {

    public:
      reporting_thread_executor(std::size_t thread_count);

//    protected:
//      bool SubmitToThread(std::function<void()> &&function) override;
    };
  }
}



#endif //AMAZON_KINESIS_PRODUCER_INTERNAL_REPORTING_THREAD_EXECUTOR_H
