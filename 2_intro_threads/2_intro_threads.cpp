#include <iostream>
#include <thread>

void print_thread_id(const char* name) {
   std::cout << name << " tid: " << std::this_thread::get_id() << std::endl;
}

int main() {

   // id of main thread
   print_thread_id("main");

   // wait for continuation
   std::cin.get();

   // spawn 3 threads and print their ids
   std::thread t1(print_thread_id, "t1");
   std::thread t2(print_thread_id, "t2");
   std::thread t3(print_thread_id, "t3");

   // don't forget to wait for threads to finish
   t1.join();
   t2.join();
   t3.join();

   // show detach / join detach / detach join
}
