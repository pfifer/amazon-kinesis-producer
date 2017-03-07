//
// Created by Pfifer, Justin on 3/6/17.
//

#include "aws/utils/io_service_executor.h"
#include <pthread.h>
#include <system_error>

static void* thread_start(void* user_context) {
  auto executor = static_cast<aws::utils::IoServiceExecutor*>(user_context);
  executor->io_service()->run();
  return user_context;
}

namespace aws {
  namespace utils {

    IoServiceThread::IoServiceThread(IoServiceExecutor *executor) {
      int res = pthread_create(&thread_id, nullptr, thread_start, executor);
      if (res != 0) {
        throw std::system_error(res, std::generic_category());
      }
    }

    IoServiceThread::~IoServiceThread() {
      pthread_detach(thread_id);
    }

    void IoServiceThread::join() {
      int res = pthread_join(thread_id, nullptr);
      if (res != 0) {
        throw std::system_error(res, std::generic_category());
      }
    }


    IoServiceExecutor::IoServiceExecutor(size_t num_threads) : io_service_(std::make_shared<boost::asio::io_service>()),
                                                               w_(*io_service_),
                                                               clean_up_cb_(
                                                                       [this] { this->clean_up(); },
                                                                       *io_service_,
                                                                       Clock::now() + std::chrono::seconds(1)) {
      for (size_t i = 0; i < num_threads; i++) {
        IoServiceThread thread(this);
        threads_.emplace_back(std::move(thread));
      }
    }

    IoServiceExecutor::~IoServiceExecutor() {
      w_.~work();
      io_service_->stop();
      for (auto &t : threads_) {
        t.join();
      }
      clean_up();
    }

    void IoServiceExecutor::submit(Func f) {
      io_service_->post(std::move(f));
    };
  }
}
