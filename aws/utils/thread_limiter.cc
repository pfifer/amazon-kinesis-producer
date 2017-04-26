//
// Created by Pfifer, Justin on 4/26/17.
//

#include "thread_limiter.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "logging.h"
#include <cstdint>

#include <sys/resource.h>

#include "thread_limiter.h"

using namespace aws::utils;

namespace {




  uint32_t config_from_env() {
    const char *env_control = std::getenv("KPL_THREAD_POOL");
    if (env_control == nullptr) {
      return 0;
    }
    std::stringstream ss;
    ss << env_control;
    uint32_t thread_count;
    ss >> thread_count;
    return thread_count;
  }

  uint32_t config_from_file() {
    std::string config_file("/app/tester/conf/thread_limit.cfg");
    std::ifstream input_file(config_file);
    if (input_file.bad()) {
      LOG(warning) << "Failed to open '" << config_file << "' for reading.";
      return 0;
    }


    uint32_t thread_limit;
    input_file >> thread_limit;
    if (input_file.fail()) {
      std::string attempted_input;
      input_file >> attempted_input;

      LOG(warning) << "Failed to extract valid thread limit from " << attempted_input;
      return 0;
    }

    return thread_limit;
  }
}

void aws::utils::set_thread_limit() {
  /*
   * This is a hack to quickly test the two different thread pools using the same code base.  It hinges off
   * a file in a known directory
   */

  uint32_t thread_count = config_from_env();
  if (thread_count == 0) {
    thread_count = config_from_file();
  }

  if (thread_count != 0) {
    rlimit rlim = { 0 };
    if (getrlimit(RLIMIT_NPROC, &rlim)) {
      LOG(error) << "Failed to retrieve NPROC rlimit";
      return;
    }
    auto old = rlim.rlim_cur;
    rlim.rlim_cur = thread_count;


    if (setrlimit(RLIMIT_NPROC, &rlim)) {
      LOG(error) << "Failed to update rlimit: " << old << " to " << rlim.rlim_cur;
    } else {
      LOG(info) << "Set thread limit from " << old << " to " << rlim.rlim_cur;
    }

  }
}