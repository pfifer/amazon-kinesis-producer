//
// Created by Pfifer, Justin on 3/7/17.
//

#include "AwsClientExecutor.h"

#include <thread>

using namespace aws::utils;

#ifdef ENABLE_THREAD_NOEXCEPT
#define EXCEPT_SIGNATURE noexcept
#else
#define EXCEPT_SIGNATURE
#endif


bool AwsClientExecutor::SubmitToThread(std::function<void()> &&fx) {
  std::thread t([fx]() EXCEPT_SIGNATURE { fx(); });
  t.detach();
  return true;
}
