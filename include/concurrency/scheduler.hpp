#ifndef CONCURRENCY_SCHEDULER_HPP
#define CONCURRENCY_SCHEDULER_HPP

#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/thread/thread.hpp>

namespace concurrency {

class scheduler {

 public:

  scheduler(int n_threads = 1) {
    assert(n_threads > 0 && "Required at least 1 thread");
    m_work.emplace(boost::ref(m_io_service));
    for (int i = 0; i < n_threads; ++i) {
      m_threads.create_thread(
          boost::bind(&boost::asio::io_service::run, &m_io_service));
    }
  }

  void stop() {
    m_work = boost::none;
    m_threads.join_all();
    m_io_service.stop();
  }

  ~scheduler() {
    stop();
  }

  boost::asio::io_service& get() {
    return m_io_service;
  }

  template<typename T>
  void post(T const& t) {
    m_io_service.post(t);
  }

  scheduler(scheduler const&) = delete;  // non construction-copyable
  scheduler &operator=(scheduler const&) = delete;  // non copyable

 private:

  boost::asio::io_service m_io_service;
  boost::optional<boost::asio::io_service::work> m_work;
  boost::thread_group m_threads;

};

}

#endif
