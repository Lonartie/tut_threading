#pragma once
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <atomic>

class BankThreadSafe {
public:
   BankThreadSafe(int64_t initial_balance_cents = 0)
         : balance_cents(initial_balance_cents) {
   }

   void deposit(int64_t cents) {
      balance_cents.fetch_add(cents);
   }

   void withdraw(int64_t cents) {
      balance_cents.fetch_sub(cents);
   }

   void print_balance(const char* cat) const {
      std::cout << "[" << cat << "] Bank balance: "
            << std::setprecision(2) << std::fixed
            << balance_cents.load() / 100.0 << " €" << std::endl;
   }

private:
   std::atomic_int64_t balance_cents = 0;
};
