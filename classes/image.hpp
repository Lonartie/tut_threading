#pragma once
#include <fstream>
#include <filesystem>

struct Pixel {
   unsigned char r = 0, g = 0, b = 0;
};

// simple image class for working with ppm files
class Image {
public:
   void read(const std::filesystem::path& filename);
   void write(const std::filesystem::path& filename) const;

   int get_width() const;
   int get_height() const;

   Pixel get_pixel(int x, int y) const;
   void set_pixel(int x, int y, const Pixel& pixel);

private:
   int width = 0;
   int height = 0;
   int max_color = 0;
   std::unique_ptr<unsigned char[]> data = nullptr;
};

void Image::read(const std::filesystem::path& filename) {
   std::ifstream stream(filename, std::ios::binary);
   if (!stream) throw std::runtime_error("Could not open file " + filename.string());

   std::string magic;
   stream >> magic;
   if (magic != "P6") throw std::runtime_error("Invalid magic number " + magic);

   stream >> width >> height >> max_color;
   if (max_color != 255) throw std::runtime_error("Invalid max color value " + std::to_string(max_color));

   data = std::make_unique<unsigned char[]>(width * height * 3);
   stream.read(reinterpret_cast<char*>(data.get()), width * height * 3);

   stream.close();
}

void Image::write(const std::filesystem::path& filename) const {
   std::ofstream stream (filename, std::ios::binary);
   if (!stream) throw std::runtime_error("Could not open file " + filename.string());

   stream << "P6\n" << width << " " << height << "\n" << max_color << "\n";
   stream.write(reinterpret_cast<const char*>(data.get()), width * height * 3);

   stream.flush();
   stream.close();
}

int Image::get_width() const {
   return width;
}

int Image::get_height() const {
   return height;
}

Pixel Image::get_pixel(int x, int y) const {
   if (x < 0 || x >= width || y < 0 || y >= height) throw std::runtime_error("Invalid pixel coordinates");
   return {data[(y * width + x) * 3], data[(y * width + x) * 3 + 1], data[(y * width + x) * 3 + 2]};
}

void Image::set_pixel(int x, int y, const Pixel& pixel) {
   if (x < 0 || x >= width || y < 0 || y >= height) throw std::runtime_error("Invalid pixel coordinates");
   data[(y * width + x) * 3] = pixel.r;
   data[(y * width + x) * 3 + 1] = pixel.g;
   data[(y * width + x) * 3 + 2] = pixel.b;
}
