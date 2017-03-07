//
// Created by Pfifer, Justin on 3/7/17.
//

#include "reporting_thread_executor.h"

using namespace aws::utils;

bool reporting_thread_executor::SubmitToThread(std::function<void()> &&function) {
  std::thread t = make_reporting_thread(std::move(function));
  t.detach();

  return true;
}