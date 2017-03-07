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

static void real_thread_proc(std::function<void()> &&fx) EXCEPT_SIGNATURE {
  fx();
}


bool AwsClientExecutor::SubmitToThread(std::function<void()> &&fx) {
  std::thread t(real_thread_proc, std::move(fx));
  t.detach();
  return true;
}
