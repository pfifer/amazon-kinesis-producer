//
// Created by Pfifer, Justin on 3/7/17.
//

#include "reporting_thread.h"


#ifdef ENABLE_THREAD_NOEXCEPT
#define EXCEPT_SIGNATURE noexcept
#else
#define EXCEPT_SIGNATURE
#endif

#include <system_error>
#include <iostream>

namespace {

  void reporting_thread_proc(std::function<void()> &&thread_function) EXCEPT_SIGNATURE {
    try {
      thread_function();
    } catch (std::system_error& err) {
      std::cerr << std::endl << "++++" << std::endl << "[INFO] Caught system_error: " << err.what() << std::endl << "----" << std::endl << std::flush;
    } catch (...) {
      std::cerr << std::endl << "++++" << std::endl << "[INFO] Caught unknown error" << std::endl << "----" << std::endl << std::flush;
    }
  }
}

aws::thread aws::utils::make_reporting_thread(std::function<void()> &&thread_function) {
  aws::thread t(reporting_thread_proc, std::move(thread_function));

  return t;
}