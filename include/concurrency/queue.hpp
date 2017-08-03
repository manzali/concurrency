#ifndef CONCURRENCY_QUEUE_HPP
#define CONCURRENCY_QUEUE_HPP

#include <queue>
#include <chrono>
#include <mutex>
#include <condition_variable>

namespace concurrency {

template<typename T>
class queue {

 public:

  void push(T const& item) {
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_queue.push(item);
    m_cond.notify_one();
  }

  void push(T&& item) {
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_queue.push(std::move(item));
    m_cond.notify_one();
  }

  void pop(T& item) {
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty()) {
      m_cond.wait(mlock);
    }
    item = std::move(m_queue.front());
    m_queue.pop();
  }

  template<typename Rep, typename Period>
  bool pop(T& item, std::chrono::duration<Rep, Period> const& timeout) {
    std::unique_lock<std::mutex> mlock(m_mutex);
    std::chrono::system_clock::time_point const tp =
        std::chrono::system_clock::now() + timeout;
    while (m_queue.empty()
        && m_cond.wait_until(mlock, tp) != std::cv_status::timeout) {
    }
    if (m_queue.empty()) {
      return false;
    }
    item = std::move(m_queue.front());
    m_queue.pop();
    return true;
  }

  queue() {
  }

  queue(queue const&) = delete;  // non construction-copyable
  queue &operator=(queue const&) = delete;  // non copyable

 private:

  std::queue<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

}

#endif
