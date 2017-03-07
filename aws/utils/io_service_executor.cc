//
// Created by Pfifer, Justin on 3/6/17.
//

#include "aws/utils/io_service_executor.h"

static void thread_start(aws::utils::IoServiceExecutor *executor) noexcept {
  executor->io_service()->run();
}

namespace aws {
  namespace utils {
    IoServiceExecutor::IoServiceExecutor(size_t num_threads) : io_service_(std::make_shared<boost::asio::io_service>()),
                                                               w_(*io_service_),
                                                               clean_up_cb_(
                                                                       [this] { this->clean_up(); },
                                                                       *io_service_,
                                                                       Clock::now() + std::chrono::seconds(1)) {
      for (size_t i = 0; i < num_threads; i++) {
        aws::thread thread(&(thread_start), this);
        threads_.emplace_back(std::move(thread));
      }
    }

    IoServiceExecutor::~IoServiceExecutor() {
      w_.~work();
      io_service_->stop();
      for (auto& t : threads_) {
        t.join();
      }
      clean_up();
    }

    void IoServiceExecutor::submit(Func f) {
      io_service_->post(std::move(f));
    };
  }
}
