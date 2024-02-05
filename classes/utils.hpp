#pragma once
#include <any>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

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