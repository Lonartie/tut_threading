// ReSharper disable CppCStyleCast
#include <iostream>
#include <vector>
#include <thread>

#include "utils.hpp"
#include "image.hpp"

constexpr auto INPUT = "/Users/lonartie/pgit/tut_threading/5_graying_parallel/image.ppm";
constexpr auto OUTPUT = "/Users/lonartie/pgit/tut_threading/5_graying_parallel/image_gray.ppm";
static auto THREADS = std::thread::hardware_concurrency();

int TIMED(main)() {
   Image image;
   std::cout << "Using " << THREADS << " threads\n";
   TIME_IT("Reading image", [&]{ image.read(INPUT); });
   TIME_IT("Processing image", [&] {

      std::vector<std::thread> threads;

      for (int i = 0; i < THREADS; i++) {
         threads.emplace_back([&, i] {
            int yincrement = image.get_height() / THREADS;
            int ystart = i * yincrement;
            int yend = (i + 1 == THREADS) ? image.get_height() : (i + 1) * yincrement;

            for (int y = ystart; y < yend; y++) {
               for (int x = 0; x < image.get_width(); x++) {
                  // no synchronization needed because each thread works on different pixels!
                  Pixel pixel = image.get_pixel(x, y);
                  unsigned char gray = ((int)pixel.r + (int)pixel.g + (int)pixel.b) / 3;
                  pixel.r = pixel.g = pixel.b = gray;
                  image.set_pixel(x, y, pixel);
               }
            }
         });
      }

      for (auto& thread : threads) {
         thread.join();
      }

   });
   TIME_IT("Writing image", [&]{ image.write(OUTPUT); });
   std::cout << "\nDone\n";

   return EXIT_SUCCESS;
}
