#pragma once
#include <any>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <functional>
#include <mutex>

#define _CAT(a,b) a##b
#define CAT(a,b) _CAT(a,b)

static std::string formatted(int64_t nanoseconds) {
   std::stringstream ss;
   ss << std::setprecision(2) << std::fixed;
   if (nanoseconds < 1'000) ss << nanoseconds << " ns";
   else if (nanoseconds < 1'000'000) ss << nanoseconds / 1'000.0 << " us";
   else if (nanoseconds < 1'000'000'000) ss << nanoseconds / 1'000'000.0 << " ms";
   else ss << nanoseconds / 1'000'000'000.0 << " s";
   return ss.str();
}

static int timed_func(int(*f)()) {
   int result;
   auto start = std::chrono::high_resolution_clock::now();
   result = f();
   auto end = std::chrono::high_resolution_clock::now();
   auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
   std::cout << "Execution time: " << formatted(nanoseconds) << std::endl;
   return result;
}

static void timed_func(const char* msg, std::function<void()> f) {
   auto start = std::chrono::high_resolution_clock::now();
   f();
   auto end = std::chrono::high_resolution_clock::now();
   auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
   std::cout << msg << " time: " << formatted(nanoseconds) << std::endl;
}

#define TIMED(...)  main_impl(); \
int main() { return timed_func(&main_impl); } \
int main_impl

#define TIME_IT(NAME, FUNC) timed_func(NAME, FUNC)

template <typename MUT>
struct _lock_iter_ {
   explicit _lock_iter_(MUT& mutex) : lock(mutex) {}
   _lock_iter_(const _lock_iter_&) = delete;
   _lock_iter_(_lock_iter_&&) = delete;
   _lock_iter_& operator=(const _lock_iter_&) = delete;
   _lock_iter_& operator=(_lock_iter_&&) = delete;
   [[nodiscard]] bool done() const { return itered; }
   void set_done() { itered = true; }
private:
   std::unique_lock<MUT> lock;
   bool itered = false;
};

#define synced_on(MUTEX) for (_lock_iter_ _iter_(MUTEX); !_iter_.done(); _iter_.set_done())
#define synced static std::mutex CAT(_tex_, __LINE__); synced_on(CAT(_tex_, __LINE__))