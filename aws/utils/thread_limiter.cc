//
// Created by Pfifer, Justin on 4/26/17.
//

#include "thread_limiter.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>
#include <memory>
#include <chrono>
#include <cstdint>
#include <exception>
#include <system_error>
#include <sys/resource.h>
#include <sys/stat.h>

#include "logging.h"

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

  uint32_t config_from_file(const std::string &config_file) {
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

  void update_thread_limit(const std::string &config_file) {
    /*
 * This is a hack to quickly test the two different thread pools using the same code base.  It hinges off
 * a file in a known directory
 */

    uint32_t thread_count = config_from_env();
    if (thread_count == 0) {
      thread_count = config_from_file(config_file);
    }

    if (thread_count != 0) {
      rlimit rlim = {0};
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

  class thread_limiter {
    using clock = std::chrono::system_clock;
    using time_point = std::chrono::time_point<clock>;

    std::unique_ptr<std::thread> update_thread_;
    time_point last_dynamic_config_update_;
    std::string static_config_file_;
    std::string dynamic_config_file_;

    std::chrono::milliseconds update_check_;



    time_point last_dynamic_update() {
      struct stat dynamic_check;
      time_t last_update;
      if (stat(dynamic_config_file_.c_str(), &dynamic_check) == -1) {
        if (errno != ENOENT) {
          throw std::system_error(errno, std::generic_category());
        }
        last_update = 0;
      } else {
        last_update = dynamic_check.st_mtimespec.tv_sec;
      }
      return clock::from_time_t(last_update);
    }

    std::string display_time_point(const time_point& tp) {
      struct tm lt;
      std::time_t tt = clock::to_time_t(tp);
      localtime_r(&tt, &lt);

      char display[255] = { 0 };

      strftime(display, sizeof(display) - 1, "%FT%T", &lt);

      std::string result(display);
      return result;
    }

    void check_update() {
      while (true) {
        std::this_thread::sleep_for(update_check_);

        LOG(info) << "Checking for changes to " << dynamic_config_file_;
        time_point last_updated = last_dynamic_update();
        if (last_updated > last_dynamic_config_update_) {
          LOG(info) << "Old Version: " << display_time_point(last_dynamic_config_update_)
                    << " -- New Version: " << display_time_point(last_updated);
          last_dynamic_config_update_ = last_updated;
          update_thread_limit(dynamic_config_file_);
        } else {
          LOG(info) << "No changes to " << dynamic_config_file_;
        }
      }
    }

  public:

    thread_limiter() : static_config_file_("/app/tester/conf/thread_limit.cfg"),
                       dynamic_config_file_("/tmp/thread_limit.cfg") {
      {
        using namespace std::chrono_literals;
        update_check_ = std::chrono::duration_cast<std::chrono::milliseconds>(10s);
      }

      last_dynamic_config_update_ = last_dynamic_update();
      update_thread_ = std::make_unique<std::thread>(&thread_limiter::check_update, this);
      update_thread_->detach();
      update_thread_limit(static_config_file_);
    }

  };

  std::shared_ptr<thread_limiter> g_thread_limiter;
}

void aws::utils::set_thread_limit() {
  if (g_thread_limiter == nullptr) {
    g_thread_limiter = std::make_shared<thread_limiter>();
  }
}