//
// Created by Pfifer, Justin on 3/7/17.
//

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

aws::thread &&aws::utils::make_reporting_thread(std::function<void()> &&thread_function) {
  aws::thread t(reporting_thread_proc, std::move(thread_function));

  return std::move(t);
}