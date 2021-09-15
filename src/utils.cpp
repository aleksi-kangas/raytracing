#include "utils.h"

#include <iostream>

#include "lodepng.h"

namespace utils {

std::array<unsigned char, 3> ColorToRGB(const Color &color, int samples_per_pixel) {
  double r = color.X();
  double g = color.Y();
  double b = color.Z();

  std::array<unsigned char, 3> rgb{};
  rgb[0] = static_cast<unsigned char>(255.999 * r);
  rgb[1] = static_cast<unsigned char>(255.999 * g);
  rgb[2] = static_cast<unsigned char>(255.999 * b);
  return rgb;
}

std::optional<std::vector<std::vector<Color>>> ReadPNG(const std::filesystem::path &path,
                                                       unsigned int &width,
                                                       unsigned int &height) {
  std::vector<unsigned char> buffer;
  unsigned int error_code = lodepng::decode(buffer, width, height, path, LCT_RGB);

  if (error_code > 0) {
    std::cerr << lodepng_error_text(error_code) << std::endl;
    return std::nullopt;
  }

  std::vector<std::vector<Color>> image(height);

  for (int row = static_cast<int>(height) - 1; row >= 0; --row) {
    for (int column = 0; column < width; ++column) {
      unsigned int index = (width * 3) * row + column * 3;
      image[row].emplace_back(
          buffer[index + 0],
          buffer[index + 1],
          buffer[index + 2]
      );
    }
  }
  return image;
}

void WritePNG(const std::filesystem::path &path,
              int width,
              int height,
              int samples_per_pixel,
              const std::vector<std::vector<Color>> &image) {
  std::vector<unsigned char> image_buffer(width * height * 3);
  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      const Color &color = image[row][column];
      std::array<unsigned char, 3> rgb = ColorToRGB(color, samples_per_pixel);
      const int kIndex = 3 * ((height - row - 1) * width + column);
      image_buffer[kIndex + 0] = rgb[0];
      image_buffer[kIndex + 1] = rgb[1];
      image_buffer[kIndex + 2] = rgb[2];
    }
  }
  unsigned int error_code = lodepng::encode(path, image_buffer, width, height, LCT_RGB);
  if (error_code > 0) {
    std::cerr << lodepng_error_text(error_code) << std::endl;
  }
}

}  // namespace utils