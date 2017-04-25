//
// Created by Pfifer, Justin on 4/24/17.
//

#include "thread_pool_selector.h"
#include <sys/stat.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "logging.h"

using namespace aws::utils;

namespace {
  enum class threading_config : int {
    NONE,
    POOLED,
    SIMPLE
  };

  threading_config from_string(const std::string &name) {
    if (name == "pooled") {
      return threading_config::POOLED;
    }
    if (name == "simple") {
      return threading_config::SIMPLE;
    }
    LOG(error) << "Invalid threading config: '" << name << "'.  Returning NONE";
    return threading_config::NONE;
  }

  threading_config config_from_env() {
    const char *env_control = std::getenv("KPL_THREAD_POOL");
    if (env_control == nullptr) {
      return threading_config::NONE;
    }
    return from_string(env_control);
  }

  threading_config config_from_file() {
    std::string config_file("/app/tester/conf/thread_model.cfg");
    std::ifstream input_file(config_file);
    if (input_file.bad()) {
      LOG(warning) << "Failed to open '" << config_file << "' for reading.";
      return threading_config::NONE;
    }


    std::stringstream config_read;
    input_file >> config_read.rdbuf();
    std::string config_value = config_read.str();

    return from_string(config_value);
  }
}

std::shared_ptr<Aws::Utils::Threading::Executor> aws::utils::make_executor() {
  /*
   * This is a hack to quickly test the two different thread pools using the same code base.  It hinges off
   * a file in a known directory
   */

  threading_config thread_config = config_from_env();
  if (thread_config == threading_config::NONE) {
    thread_config = config_from_file();
  }


  switch (thread_config) {
    case threading_config::POOLED:
      LOG(info) << "Using threading model: POOLED";
      return std::make_shared<reporting_thread_executor>(90);
    case threading_config::NONE:
      LOG(error) << "Failed to get threading config default to simple";
      // Fallthrough expected
    case threading_config::SIMPLE:
      LOG(info) << "Using threading model: SIMPLE";
      return std::make_shared<simple_executor>();
  }
}