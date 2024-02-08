#include <iostream>
#include <vector>
#include <thread>
#include <random>

std::vector<double> create_vec();
double threaded_sum(const std::vector<double>& vec);

int main() {
   double sum = threaded_sum(create_vec());
   std::cout << "Sum: " << sum << std::endl;
}

/* EXAMPLES HERE */

double threaded_sum(const std::vector<double>& vec) {
   const int threadcount = std::thread::hardware_concurrency();
   std::vector<double> partial_sums(threadcount, 0);
   std::vector<std::thread> threads;
   std::mutex console_tex;

   for (int i = 0; i < threadcount; i++) {
      threads.emplace_back([&, i] {
         int from = i * vec.size() / threadcount;
         int to = (i + 1 == threadcount) ? vec.size() : (i + 1) * vec.size() / threadcount;
         double threadsum = 0;

         for (int j = from; j < to; j++) {
            threadsum += vec[j];
         } {
            std::lock_guard lock(console_tex);
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

/* END OF EXAMPLES */

std::vector<double> create_vec() {
   std::vector<double> vec;
   srand(42);
   for (int i = 0; i < 1'000'000; i++) {
      vec.push_back(rand() / (double)RAND_MAX);
   }
   return vec;
}
