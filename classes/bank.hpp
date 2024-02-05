#pragma once
#include <iostream>
#include <cstdint>
#include <iomanip>

class Bank {
public:
   int64_t get_balance() const {
      return balance_cents;
   }

   void set_balance(int64_t cents) {
      balance_cents = cents;
   }

   void print_balance(const char* cat) const {
      std::cout << "[" << cat << "] Bank balance: "
            << std::setprecision(2) << std::fixed
            << balance_cents / 100.0 << " €" << std::endl;
   }

private:
   int64_t balance_cents = 0;
};
