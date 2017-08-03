#include <thread>
#include <iostream>

#include "concurrency/queue.hpp"

namespace {
// std::make_unique is available starting from c++14
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&& ...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}

class obj {

  int m_value;

 public:

  obj()
      : m_value(0) {
    std::cout << "default constructor\n";
  }

  obj(int value)
      : m_value(value) {
    std::cout << "constructor with parameter\n";
  }

  obj(obj&& other) {
    std::swap(m_value, other.m_value);
    std::cout << "move constructor\n";
  }

  obj(obj const& other)
      : m_value(other.m_value) {
    std::cout << "copy constructor\n";
  }
  void operator=(obj const& other) {
    m_value = other.m_value;
    std::cout << "operator=\n";
  }

  obj& operator=(obj&& other) {
    std::swap(m_value, other.m_value);
    std::cout << "move assigned\n";
    return *this;
  }
  ~obj() {
    std::cout << "destructor\n";
  }
  int value() {
    return m_value;
  }
};

int main() {

  concurrency::queue<obj> queue;

  int const n = 5;

  std::thread th2([&]() {
    int counter = 0;
    obj p;
    while (counter < n) {
      if(queue.pop(p, std::chrono::milliseconds(100))) {
        std::cout << "retrivied " << p.value() << std::endl;
        ++counter;
      }
      else {
        std::cout << "no item retrieved" << std::endl;
      }
    }
  });

  std::thread th1([&]() {
    for (int i = 0; i < n; ++i) {
      obj p(i);
      queue.push(std::move(p));
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  });

  th1.join();

  th2.join();

  return EXIT_SUCCESS;
}
