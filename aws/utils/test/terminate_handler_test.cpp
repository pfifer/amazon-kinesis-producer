//
// Created by Pfifer, Justin on 3/2/17.
//

#include "aws/utils/signal_handler.h"

#include <system_error>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

void verify_terminate_handler() {
  std::cout << "Chained termination handler was called as expected" << std::endl << std::flush;
}

class Test3 {
public:
  void test(int how) {
    if (how > 100) {
      throw 10;
    }
    if (how > 1000) {
      std::stringstream ss;
      ss << "How is greater than 1000: " << how;
      throw std::runtime_error(ss.str());
    }
    if (how > 9000) {
      std::stringstream ss;
      ss << "What does the scouter say? It's over 9000!!: " << how;
      throw std::system_error(std::make_error_code(std::errc::no_buffer_space));
    }
    std::cout << "How is below 100 no exception" << std::endl << std::flush;
  }
};

class Test2 {
public:
  Test3 test3;
  void level_2(int how) {
    try {
      test3.test(how);
    } catch (int code) {
      std::cout << "Caught code: " << code << std::endl << std::flush;
      std::stringstream message;
      message << "Caught code: " << code;
      throw std::runtime_error(message.str());
    }
  }

  void test_no(int how) {
    test3.test(how);
  }
};

class Test1 {
public:
  Test2 test2;
  void level_1(int how) {
    try {
      test2.level_2(how);
    } catch (std::runtime_error& err) {
      std::cout << "Caught runtime_error: " << err.what() << std::endl << std::flush;
    }
  }

  void test_no(int how) {
    test2.test_no(how);
  }
};



int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Need the mode, and the value for how. argc: " << argc << std::endl;
    exit(1);
  }
  std::set_terminate(&verify_terminate_handler);
  aws::utils::setup_stack_trace(argv[0]);
  std::string mode(argv[1]);
  int how = std::atoi(argv[2]);
  Test1 test;

  if (mode == "handled") {
    test.level_1(10);
    test.level_1(101);
    test.level_1(1001);
    std::cout << "Now ready to cause unhandled exception" << std::endl << std::flush;
    test.level_1(9001);
  } else if (mode == "unhandled") {
    test.test_no(how);
  } else {
    std::cerr << "Unknown mode: " << mode << std::endl;
  }

}
