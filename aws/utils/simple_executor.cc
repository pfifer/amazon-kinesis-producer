//
// Created by Pfifer, Justin on 4/24/17.
//

#include "simple_executor.h"

#include <thread>

using namespace aws::utils;

bool simple_executor::SubmitToThread(std::function<void()> &&function) {
  std::thread t = make_reporting_thread(std::move(function));
  t.detach();

  return true;
}