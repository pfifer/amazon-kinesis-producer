//
// Created by Pfifer, Justin on 3/2/17.
//

#include "aws/utils/signal_handler.h"
#include "aws/build_info.h"
#include <iostream>
#include <thread>
#include <pthread.h>
#include <getopt.h>
#include <system_error>
#include "aws/utils/reporting_thread.h"


#include "levels/terminate_level_1.h"

std::terminate_handler existing_handler = nullptr;

void verify_terminate_handler() {
  std::cout << "Chained termination handler was called as expected" << std::endl << std::flush;

  if (existing_handler) {
    std::cout << "Existing handler is present, chaining to it." << std::endl << std::flush;
    (*existing_handler)();
    std::cout << "Existing handler has returned." << std::endl << std::flush;
  }
}

struct Configuration {
  int how;
  bool in_catch = false;
  bool is_rethrown = false;
  int start_level = 2;
  bool threaded = false;
  bool enable_stack = true;
  bool use_pthread = false;
};

struct option options[] = {
        {"how", required_argument, nullptr, 'o'},
        {"in-catch", no_argument, nullptr, 'c'},
        {"rethrow", no_argument, nullptr, 'r'},
        {"start-level", required_argument, nullptr, 's'},
        {"threaded", no_argument, nullptr, 't'},
        {"disable-stack", no_argument, nullptr, 'd'},
        {"use-pthread", no_argument, nullptr, 'p'},
        {nullptr, 0, nullptr, 0}
};

void run_test(const Configuration &config) {
  aws::utils::terminate_level_1 test;
  switch (config.start_level) {
    case 0:
      test.local_throw(config.how);
      break;
    case 1:
      test.level_2_throw(config.how, config.in_catch, config.is_rethrown);
      break;
    case 2:
      test.level_3_throw(config.how, config.in_catch, config.is_rethrown);
      break;
    default:
      std::cerr << "Unknown level: " << config.start_level << ". " << "Using deepest level" << std::endl;
      test.level_3_throw(config.how, config.in_catch, config.is_rethrown);

  }
}

void *test_pthread_start(void *ucontext) {
  auto config = static_cast<Configuration *>(ucontext);
  run_test(*config);
  return ucontext;
}


int main(int argc, char **argv) {

  std::cout << "Build Version: " << BUILD_VERSION << std::endl;

  Configuration config;
  int ch;
  while ((ch = getopt_long(argc, argv, "o:s:crtdp", options, nullptr)) != -1) {
    switch (ch) {
      case 'o':
        config.how = std::atoi(optarg);
        break;
      case 'c':
        config.in_catch = true;
        break;
      case 'r':
        config.is_rethrown = true;
        break;
      case 's':
        config.start_level = std::atoi(optarg);
        break;
      case 't':
        config.threaded = true;
        break;
      case 'd':
        config.enable_stack = false;
        break;
      case 'p':
        config.use_pthread = true;
        break;
      default:
        std::cerr << "Unknown option '" << ch << "'" << std::endl;
        exit(1);
    }
  }
  argc -= optind;
  argv += optind;

  if (config.enable_stack) {
    existing_handler = std::get_terminate();
    std::set_terminate(&verify_terminate_handler);
    aws::utils::setup_stack_trace(argv[0]);
  }

  if (config.threaded) {
    if (config.use_pthread) {
      pthread_t thread_id;
      int res = pthread_create(&thread_id, nullptr, test_pthread_start, &config);
      if (res != 0) {
        throw std::system_error(res, std::generic_category());
      }
      pthread_join(thread_id, nullptr);
    } else {
      std::thread thread = aws::utils::make_reporting_thread([&config] { run_test(config); });
      thread.join();
    }
  } else {
    try {
      run_test(config);
    } catch (std::runtime_error &err) {
      std::cerr << "Caught runtime error: " << err.what() << std::endl << std::flush;
      throw err;
    }
  }


}
