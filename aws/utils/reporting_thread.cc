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

#include "reporting_thread.h"


#ifdef ENABLE_THREAD_NOEXCEPT
#define EXCEPT_SIGNATURE noexcept
#else
#define EXCEPT_SIGNATURE
#endif

namespace {

  void reporting_thread_proc(std::function<void()> &&thread_function) EXCEPT_SIGNATURE {
    thread_function();
  }
}

aws::thread aws::utils::make_reporting_thread(std::function<void()> &&thread_function) {
  aws::thread t(reporting_thread_proc, std::move(thread_function));

  return t;
}