#include <iostream>
#include <cstdint>
#include <thread>

#include "bank.hpp"

int main() {
   Bank bank;
   bank.set_balance(100 * 100);

   bank.print_balance("BEFORE");

   // Child has 100 euros

   std::thread child([&]() {
      for (uint64_t i = 0; i < 1'000'000; i++) {
         // child withdraws 10 euros for to buy a toy
         int64_t old_balance = bank.get_balance();
         int64_t new_balance = old_balance - 10 * 100;
         bank.set_balance(new_balance);
      }
   });

   std::thread mother([&]() {
      for (uint64_t i = 0; i < 1'000'003; i++) {
         // mother deposits 10 euros to the bank
         int64_t old_balance = bank.get_balance();
         int64_t new_balance = old_balance + 10 * 100;
         bank.set_balance(new_balance);
      }
   });

   // wait for child and mother to finish
   child.join();
   mother.join();

   // balance should be 100 euros
   bank.print_balance("AFTER");
}
