// Copyright 2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Amazon Software License (the "License").
// You may not use this file except in compliance with the License.
// A copy of the License is located at
//
//  http://aws.amazon.com/asl
//
// or in the "license" file accompanying this file. This file is distributed
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied. See the License for the specific language governing
// permissions and limitations under the License.

#include "signal_handler.h"
#include "writer_methods.h"
#include "backtrace/backtrace.h"
#include "aws/build_info.h"

#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <exception>
#include <system_error>
#include <atomic>
#include <mutex>
#include <thread>


#define MESSAGE_AND_SIZE(msg) (msg), (sizeof(msg) - 1)
static size_t signal_message_sizes[NSIG];

static std::atomic<bool> throw_exception(false);
static std::atomic<bool> in_signal_processing(false);

void write_signal_description(int signal) {
  if (signal <= 0 || signal >= NSIG) {
    WRITE_MESSAGE("Can't Find Signal Description for ")
    WRITE_NUM_CHECKED(signal, "Negative Signal")
  } else {
    if (signal_message_sizes[signal]) {
      write(STDERR_FILENO, sys_siglist[signal], signal_message_sizes[signal]);
    } else {
      WRITE_MESSAGE("Signal had NULL message.")
    }
  }
}

static void write_stack_trace() {
  WRITE_MESSAGE("---BEGIN STACK TRACE---\n");
  aws::utils::backtrace::stack_trace_for_signal();
  WRITE_MESSAGE("---END STACK TRACE---\n");
}

static void write_error_header() {
  WRITE_MESSAGE("[ERROR]\n");
  WRITE_MESSAGE("---BEGIN ERROR----\n");
}

static void write_info_header(const char *message, size_t message_size) {
  WRITE_MESSAGE("[INFO]\n");
  WRITE_MESSAGE("---BEGIN INFO---\n");
  ::aws::utils::writer::write_message(message, message_size);
}

static void write_error_tail() {
  WRITE_MESSAGE("---END ERROR---\n");
}

static void write_info_tail() {
  WRITE_MESSAGE("---END INFO---\n");
}

static void write_report_start() {
  WRITE_MESSAGE("\n++++\n");
}

static void write_report_end() {
  WRITE_MESSAGE("----\n");
}

static void write_build_info() {
  WRITE_MESSAGE("Build: "
                        BUILD_VERSION
                        "\n");
}

static void acquire_and_write_stack_trace() {

}

static void signal_handler(int, siginfo_t *info, void *) {

  write_report_start();
  if (info->si_signo == SIGUSR1) {
    write_info_header(MESSAGE_AND_SIZE("User Requested Stack Trace\n"));
  } else {
    write_error_header();
  }
  write_build_info();
  WRITE_MESSAGE("Signal: ");

  switch (info->si_signo) {
    case SIGQUIT:
      WRITE_MESSAGE("SIGQUIT");
      break;
    case SIGILL:
      WRITE_MESSAGE("SIGILL: ");
      WRITE_MESSAGE(" Code: (");
      WRITE_CODE(info->si_code);
      WRITE_MESSAGE(") Address: ");
      WRITE_POINTER(info->si_addr);
      break;
    case SIGBUS:
      WRITE_MESSAGE("SIGBUS");
      WRITE_MESSAGE(" Code: (");
      WRITE_CODE(info->si_code);
      WRITE_MESSAGE(") Address: ");
      WRITE_POINTER(info->si_addr);
      break;
    case SIGSEGV:
      WRITE_MESSAGE("SIGSEGV");
      WRITE_MESSAGE(" Code: (");
      WRITE_CODE(info->si_code);
      WRITE_MESSAGE(") Address: ");
      WRITE_POINTER(info->si_addr);
      break;
    case SIGABRT:
      WRITE_MESSAGE("SIGABRT")
      break;
    case SIGUSR1:
      WRITE_MESSAGE("SIGUSR1");
      break;
    default:
      WRITE_MESSAGE("Unhandled Signal(");
      WRITE_NUM_CHECKED(info->si_signo, "Negative Signal?");
      WRITE_MESSAGE(")");
      break;
  }
  WRITE_MESSAGE("\n");
  WRITE_MESSAGE("Description: ");
  write_signal_description(info->si_signo);
  WRITE_MESSAGE("\n");


  if (info->si_signo == SIGUSR1) {
    write_info_tail();
  } else {
    write_error_tail();
  }

  write_report_end();

  if (info->si_signo != SIGUSR1) {
    struct sigaction restore_abort = {};
    restore_abort.sa_handler = SIG_DFL;
    sigaction(SIGABRT, &restore_abort, NULL);

    abort();
  }
  in_signal_processing.store(false);
}

static void print_stack_trace_message(const char *message) {
  write_report_start();
  write_info_header(message, strlen(message));
  write_build_info();
  write_stack_trace();
  write_info_tail();
  write_report_end();
}

static std::terminate_handler existing_handler = nullptr;
static std::mutex exception_backtrace_mutex;


static void report_terminate() {

  bool locked = false;
  try {
    exception_backtrace_mutex.lock();
    locked = true;
  } catch (...) {
    WRITE_MESSAGE("Failed to acquire lock, might get messy\n");
  }

  write_report_start();
  write_error_header();
  WRITE_MESSAGE("Terminate Called for unhandled exception.\n");
  write_build_info();
  if (!in_signal_processing.load()) {
    write_stack_trace();
  }
  write_error_tail();
  write_report_end();
  if (locked) {
    exception_backtrace_mutex.unlock();
  }
  //
  // Sleep for 10ms to allow any other activity before aborting.
  //
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(100ms);

  //
  // Chain to the next handler, which will usually be the default std::terminate handler.
  //
  if (existing_handler != nullptr) {
    (*existing_handler)();
  }
  //
  // Haven't abort yet, so should go ahead and abort
  //
  std::abort();
}

static void install_termination_handler() {
  existing_handler = std::get_terminate();
  std::set_terminate(&report_terminate);
}

#ifdef DEBUG
static void usr2_signal_handler(int, siginfo_t *info, void*) {
  write_report_start();
  if (info->si_signo == SIGUSR2) {
    write_info_header(MESSAGE_AND_SIZE("Got SIGUSR2 Triggering Exception\n"));
    write_build_info();
    throw_exception.store(true);
    write_info_tail();
  } else {
    write_error_header();
    WRITE_MESSAGE("Unknown signal: ");
    WRITE_CODE(info->si_signo);
    WRITE_MESSAGE("\n")
    write_error_tail();
  }
  write_report_end();
}
#endif

namespace aws {
  namespace utils {
    void setup_stack_trace(const char *exe) {
      aws::utils::backtrace::initialize(exe);
      signal_message_sizes[0] = 0;
      for (int i = 1; i < NSIG; ++i) {
        if (sys_siglist[i]) {
          signal_message_sizes[i] = strlen(sys_siglist[i]);
        }
      }
      sigset_t mask;
      sigemptyset(&mask);
      sigaddset(&mask, SIGQUIT);
      sigaddset(&mask, SIGILL);
      sigaddset(&mask, SIGBUS);
      sigaddset(&mask, SIGSEGV);
      sigaddset(&mask, SIGUSR1);
      sigaddset(&mask, SIGABRT);

      struct sigaction action;
      action.sa_sigaction = &signal_handler;
      action.sa_flags = SA_SIGINFO;
      action.sa_mask = mask;

      sigaction(SIGQUIT, &action, NULL);
      sigaction(SIGILL, &action, NULL);
      sigaction(SIGBUS, &action, NULL);
      sigaction(SIGSEGV, &action, NULL);


      //
      // This enables customers to trigger a stack trace at any time by sending SIGUSR1 to the Kinesis Producer
      // PID.  Unfortunately there is no way to control which thread actually handles the signal, so this is of
      // questionable utility.
      //
      // TODO: Change the overall thread handling to allow triggering a stack trace from every thread.
      //
      sigaction(SIGUSR1, &action, NULL);

#ifdef DEBUG
      //
      // We use SIGUSR2 to trigger specific debug actions
      //
      sigset_t usr2_mask;
      sigemptyset(&mask);
      sigaddset(&mask, SIGUSR2);
      struct sigaction usr2_action = {0};
      usr2_action .sa_sigaction = &usr2_signal_handler;
      usr2_action .sa_flags = SA_SIGINFO;
      usr2_action .sa_mask = usr2_mask;

      sigaction(SIGUSR2, &usr2_action, NULL);
#endif
      //
      // Ignoring this since curl/OpenSSL can trigger them and something is wrong with it's ignore
      //
      struct sigaction pipe_action = {};
      pipe_action.sa_handler = SIG_IGN;
      sigaction(SIGPIPE, &pipe_action, NULL);

      //
      // Add a termination handler to try and report stack traces
      //
      install_termination_handler();
    }


#ifdef DEBUG
    void throw_test_exception() {
      bool expected_state = true;
      if (throw_exception.compare_exchange_strong(expected_state, false, std::memory_order::memory_order_acq_rel, std::memory_order::memory_order_acq_rel)) {
        print_stack_trace_message("Exception throw start requested: " BUILD_VERSION "\n");
        throw std::system_error(std::make_error_code(std::errc::already_connected));
      }
    }
#endif
  }
}
