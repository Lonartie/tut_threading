#include <iostream>
#include <cstdint>

#include "bank.hpp"

int main() {
   Bank bank;
   bank.set_balance(100 * 100);

   bank.print_balance("BEFORE");

   // Child has 100 euros

   for (uint64_t i = 0; i < 1'000'000; i++) {
      // child withdraws 10 euros for to buy a toy
      bank.set_balance(bank.get_balance() - 10 * 100);

      // mother deposits 10 euros to the bank
      bank.set_balance(bank.get_balance() + 10 * 100);
   }

   // mother deposits 3 times 10 euros to the bank
   bank.set_balance(bank.get_balance() + 10 * 100);
   bank.set_balance(bank.get_balance() + 10 * 100);
   bank.set_balance(bank.get_balance() + 10 * 100);

   // balance should be 130 euros
   bank.print_balance("AFTER");
}
