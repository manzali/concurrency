#ifndef CONCURRENCY_SPIN_LOCK_HPP
#define CONCURRENCY_SPIN_LOCK_HPP

#include <atomic>
#include <thread>

// http://en.cppreference.com/w/cpp/atomic/atomic_flag

namespace concurrency {

namespace spin {

class mutex {
  friend class scoped_lock;
  std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
 public:
  mutex() {
  }
  mutex(mutex const&) = delete;  // non construction-copyable
  mutex &operator=(mutex const&) = delete;  // non copyable
};

class scoped_lock {
  std::atomic_flag& m_flag;
 public:
  scoped_lock(mutex& m)
      : m_flag(m.m_flag) {
    while (m_flag.test_and_set(std::memory_order_acquire)) {
      std::this_thread::yield();  // it does the magic
    }
  }
  ~scoped_lock() {
    m_flag.clear(std::memory_order_release);
  }
  scoped_lock(scoped_lock const&) = delete;  // non construction-copyable
  scoped_lock &operator=(scoped_lock const&) = delete;  // non copyable
};

}

}

#endif
