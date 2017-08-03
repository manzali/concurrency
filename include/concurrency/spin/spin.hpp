#ifndef CONCURRENCY_SPIN_LOCK_HPP
#define CONCURRENCY_SPIN_LOCK_HPP

#include <atomic>
#include <thread>

// http://en.cppreference.com/w/cpp/atomic/atomic_flag

namespace concurrency {

namespace spin {

class mutex {
  friend class scoped_lock;
  std::atomic<bool> m_mutex;
 public:
  mutex()
      : m_mutex(false) {
  }
  mutex(mutex const&) = delete;  // non construction-copyable
  mutex &operator=(mutex const&) = delete;  // non copyable
};

class scoped_lock {
  std::atomic<bool>& m_mutex;
 public:
  scoped_lock(mutex& m)
      : m_mutex(m.m_mutex) {
    while (std::atomic_exchange_explicit(
        &m_mutex,
        true,
        std::memory_order_acquire)) {
      std::this_thread::yield();  // it does the magic
    }
  }
  ~scoped_lock() {
    std::atomic_store_explicit(&m_mutex, false, std::memory_order_release);
  }
  scoped_lock(scoped_lock const&) = delete;  // non construction-copyable
  scoped_lock &operator=(scoped_lock const&) = delete;  // non copyable
};

}

}

#endif
