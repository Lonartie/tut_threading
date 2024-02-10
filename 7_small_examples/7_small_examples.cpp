#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <omp.h>
#include <algorithm>
#include <bits/ranges_algo.h>

#include "utils.hpp"

std::vector<double> create_vec();

double threaded_sum(const std::vector<double>& vec);
double threaded_sum_omp(const std::vector<double>& vec);
double threaded_sum_omp_optimized(const std::vector<double>& vec);

uint64_t threaded_sum_bench(const std::vector<double>& vec, uint32_t threadcount);

int main() {
   // 1. Sum of all elements in a vector
   std::cout << "TEST 1: Sum of all elements in a vector\n";
   double sum = threaded_sum_omp(create_vec());
   std::cout << "Sum: " << sum << std::endl;

   // 2. Benchmark across num threads
   std::cout << "\nTEST 2: Benchmark across num threads\n";
   std::unordered_map<uint32_t, int64_t> times;
   for (auto tc: {1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 32}) {
      times.emplace(tc, threaded_sum_bench(create_vec(), tc));
   }
   auto min_time_tc = std::ranges::min_element(times, {}, [](const auto& p) { return p.second; })->first;
   std::cout << "\nMin time with " << min_time_tc << " threads = " << formatted(times.at(min_time_tc)) << std::endl;
}

/* EXAMPLES HERE */

double threaded_sum(const std::vector<double>& vec) {
   const unsigned threadcount = std::thread::hardware_concurrency();
   std::vector<double> partial_sums(threadcount, 0);
   std::vector<std::thread> threads;

   for (int i = 0; i < threadcount; i++) {
      threads.emplace_back([&, i] {
         int from = i * vec.size() / threadcount;
         int to = (i + 1 == threadcount) ? vec.size() : (i + 1) * vec.size() / threadcount;
         double threadsum = 0;

         for (int j = from; j < to; j++) {
            threadsum += vec[j];
         }

         synced {
            std::cout << "Thread " << i << " sum: " << threadsum << std::endl;
         }

         partial_sums[i] = threadsum;
      });
   }

   for (auto& t: threads) {
      t.join();
   }

   double sum = 0;
   for (auto& p: partial_sums) {
      sum += p;
   }
   return sum;
}

double threaded_sum_omp(const std::vector<double>& vec) {
   double sum = 0;
#pragma omp parallel num_threads(std::thread::hardware_concurrency())
   {
      double local_sum = 0;

#pragma omp for
      for (int i = 0; i < vec.size(); i++) {
         local_sum += vec[i];
      }

#pragma omp critical
      {
         sum += local_sum;
         std::cout << "Thread " << omp_get_thread_num() << " sum: " << local_sum << std::endl;
      }
   }
   return sum;
}

double threaded_sum_omp_optimized(const std::vector<double>& vec) {
   double sum = 0;
#pragma omp parallel for reduction(+:sum) num_threads(std::thread::hardware_concurrency())
   for (int i = 0; i < vec.size(); i++) {
      sum += vec[i];
   }
   return sum;
}

uint64_t threaded_sum_bench(const std::vector<double>& vec, uint32_t threadcount) {
   auto begin = std::chrono::high_resolution_clock::now();

   double sum = 0;
#pragma omp parallel for num_threads(threadcount) reduction(+:sum)
   for (int i = 0; i < vec.size(); i++) {
      sum += vec[i];
   }

   auto end = std::chrono::high_resolution_clock::now();
   auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
   std::cout << "Sum " << threadcount << " threads: " << formatted(nanoseconds) << "  sum = " << sum << std::endl;
   return nanoseconds;
}

/* END OF EXAMPLES */

std::vector<double> create_vec() {
   static std::vector<double> vec = []() {
      std::vector<double> vec;
      srand(42);
      for (int i = 0; i < 100'000'000; i++) {
         vec.push_back(rand() / (double) RAND_MAX);
      }
      return vec;
   }();
   return vec; // return copy each time because vector is too large to be created multiple times
}
