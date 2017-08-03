#include <thread>
#include <chrono>
#include <iostream>
#include <random>
#include <atomic>

#include <functional> // std::bind

#include "concurrency/scheduler.hpp"

std::atomic<unsigned int> inside(0);

void pi_cycle(unsigned int sub_iter) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis;
  for (unsigned int i = 0; i < sub_iter; ++i) {
    double const x = dis(gen);
    double const y = dis(gen);
    if (x * x + y * y < 1.0) {
      ++inside;
    }
  }
}

int main() {

  int const n_threads = 1;
  unsigned int const iter = 1024 * 1024 * 16;

  concurrency::scheduler s(n_threads);

  unsigned int sub_iter = iter / n_threads;

  auto start_time = std::chrono::high_resolution_clock::now();

  for (unsigned int i = 0; i < n_threads; ++i) {
    s.post(std::bind(pi_cycle, sub_iter));
  }
  s.stop();

  auto end_time = std::chrono::high_resolution_clock::now();

  double const elapsed_seconds = std::chrono::duration<double>(
      end_time - start_time).count();

  double pi = inside / static_cast<double>(iter) * 4.0;

  std::cout << "found pi = "
            << pi
            << " with "
            << iter
            << " iterations in "
            << elapsed_seconds
            << " s\n";

  return EXIT_SUCCESS;
}
