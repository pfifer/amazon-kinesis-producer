//
// Created by Pfifer, Justin on 3/2/17.
//

#include "aws/utils/signal_handler.h"

#include <system_error>
#include <exception>
#include <iostream>

void verify_terminate_handler() {
  std::cout << "Chained termination handler was called as expected" << std::endl;
}

int main(int argc, char** argv) {
  std::set_terminate(&verify_terminate_handler);
  aws::utils::setup_stack_trace(argv[0]);
  std::cout << "Preparing to throw an unhandled exception." << std::endl;

  throw std::system_error(std::make_error_code(std::errc::invalid_argument));

}
