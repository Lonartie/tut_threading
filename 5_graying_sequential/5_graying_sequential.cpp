#include <iostream>

#include "utils.hpp"
#include "image.hpp"

constexpr auto INPUT = "/Users/lonartie/pgit/tut_threading/4_graying_sequential/image.ppm";
constexpr auto OUTPUT = "/Users/lonartie/pgit/tut_threading/4_graying_sequential/image_gray.ppm";

int TIMED(main)() {
   Image image;
   TIME_IT("Reading image", [&]{ image.read(INPUT); });
   TIME_IT("Processing image", [&] {

      for (int y = 0; y < image.get_height(); y++) {
         for (int x = 0; x < image.get_width(); x++) {
            Pixel pixel = image.get_pixel(x, y);
            unsigned char gray = (pixel.r + pixel.g + pixel.b) / 3;
            pixel.r = pixel.g = pixel.b = gray;
            image.set_pixel(x, y, pixel);
         }
      }

   });
   TIME_IT("Writing image", [&]{ image.write(OUTPUT); });
   std::cout << "\nDone\n";

   return EXIT_SUCCESS;
}