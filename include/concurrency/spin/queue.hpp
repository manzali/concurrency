#ifndef CONCURRENCY_SPIN_QUEUE_HPP
#define CONCURRENCY_SPIN_QUEUE_HPP

#include <queue>

#include "concurrency/spin/mutex.hpp"

namespace concurrency {

namespace spin {

template<typename T>
class queue {

 private:

  spin::mutex m_mutex;
  std::queue<T> m_queue;

 public:

  void push(T const& item) {
    spin::scoped_lock lock(m_mutex);
    m_queue.push(item);
  }

  void push(T&& item) {
    spin::scoped_lock lock(m_mutex);
    m_queue.push(std::move(item));
  }

  void pop(T& item) {
    while (!try_pop(item))
      ;
  }

  bool try_pop(T& item) {
    spin::scoped_lock lock(m_mutex);
    if (m_queue.empty()) {
      return false;
    }
    item = std::move(m_queue.front());
    m_queue.pop();
    return true;
  }

  size_t size() {
    spin::scoped_lock lock(m_mutex);
    size_t size = m_queue.size();
    return size;
  }

  queue() {
  }

  queue(queue const&) = delete;  // non construction-copyable
  queue &operator=(queue const&) = delete;  // non copyable
};

}

}

#endif
