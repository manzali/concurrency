#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <thread>

#include "concurrency/spin/queue.hpp"

#define ITER 10

concurrency::spin::queue<int> spin_queue;

void my_producer(std::vector<int> const &rand_vect) {
  std::for_each(
      std::begin(rand_vect),
      std::end(rand_vect),
      [&](int item) {spin_queue.push(std::move(item));});
}

void my_consumer(std::vector<int> const &rand_vect) {
  int const n = rand_vect.size();
  int sum = 0;
  for (int i = 0; i < n; ++i) {
    int item = 0;
    spin_queue.pop(item);
    sum += item;
  }
  std::cout << "my sum:  " << sum << std::endl;
}

int main(/*int argc, char* argv[]*/) {

  std::vector<int> rand_vect(1024 * 1024 * 8);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis;

  std::for_each(
      std::begin(rand_vect),
      std::end(rand_vect),
      [&](int &item) {item = dis(gen);});

  // sum of all items
  int const sum = std::accumulate(
      std::begin(rand_vect),
      std::end(rand_vect),
      0);
  std::cout << "vec sum: " << sum << std::endl;

  for (int i = 0; i < ITER; ++i) {

    std::cout << std::endl;

    // start with my queue
    {
      auto start_time = std::chrono::high_resolution_clock::now();
      std::thread th1(my_producer, rand_vect);
      std::thread th2(my_consumer, rand_vect);
      th1.join();
      th2.join();
      auto end_time = std::chrono::high_resolution_clock::now();
      double const elapsed_seconds = std::chrono::duration<double>(
          end_time - start_time).count();
      std::cout << "my time: " << elapsed_seconds << " s\n";
    }

  }

  return EXIT_SUCCESS;
}
