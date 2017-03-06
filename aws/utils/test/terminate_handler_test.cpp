//
// Created by Pfifer, Justin on 3/2/17.
//

#include "aws/utils/signal_handler.h"

#include <iostream>
#include <thread>
#include <getopt.h>


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
};

struct option options[] = {
        {"how",           required_argument, nullptr, 'o'},
        {"in-catch",      no_argument,       nullptr, 'c'},
        {"rethrow",       no_argument,       nullptr, 'r'},
        {"start-level",   required_argument, nullptr, 's'},
        {"threaded",      no_argument,       nullptr, 't'},
        {"disable-stack", no_argument,       nullptr, 'd'},
        {nullptr, 0,                         nullptr, 0}
};

#ifdef DISABLE_THREAD_EXCEPT
#define EXCEPT_SIGNATURE noexcept
#else
#define EXCEPT_SIGNATURE
#endif


void run_test(const Configuration &config) EXCEPT_SIGNATURE {
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


int main(int argc, char **argv) {


  Configuration config;
  int ch;
  while ((ch = getopt_long(argc, argv, "o:s:crtd", options, nullptr)) != -1) {
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
    std::thread thread(run_test, config);
    thread.join();
  } else {
    try {
      run_test(config);
    } catch (std::runtime_error &err) {
      std::cerr << "Caught runtime error: " << err.what() << std::endl << std::flush;
      throw err;
    }
  }


}
