#include <iostream>
#include <cstdint>
#include <thread>

#include "bank_thread_safe.hpp"

int main() {
   BankThreadSafe bank(100 * 100);

   bank.print_balance("BEFORE");

   // Child has 100 euros

   std::thread child([&]() {
      for (uint64_t i = 0; i < 1'000'000; i++) {
         bank.withdraw(10 * 100);
      }
   });

   std::thread mother([&]() {
      for (uint64_t i = 0; i < 1'000'003; i++) {
         bank.deposit(10 * 100);
      }
   });

   // wait for child and mother to finish
   child.join();
   mother.join();

   // balance should be 100 euros
   bank.print_balance("AFTER");
}
