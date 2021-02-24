#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>

constexpr int tileSize = 64;

struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

constexpr Pixel backgroundColor{0, 0, 0, 0};

struct Image
{
  explicit Image(const std::string &a_path);
  Image(int a_width, int a_height, int a_channels);

  int Save(const std::string &a_path);

  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return  data; }

  Pixel GetPixel(int x, int y) { return data[width * y + x];}
  void  PutPixel(int x, int y, const Pixel &pix) { data[width * y + x] = pix; }

  ~Image();

private:
  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  bool self_allocated = false;
};

static Pixel Blend(Pixel old_pixel, Pixel new_pixel) {
    new_pixel.r = new_pixel.a / 255.0 * (new_pixel.r - old_pixel.r) + old_pixel.r;
    new_pixel.g = new_pixel.a / 255.0 * (new_pixel.g - old_pixel.g) + old_pixel.g;
    new_pixel.b = new_pixel.a / 255.0 * (new_pixel.b - old_pixel.b) + old_pixel.b;
    new_pixel.a = 255;
    return new_pixel;
}

#endif //MAIN_IMAGE_H
