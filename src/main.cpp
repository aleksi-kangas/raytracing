#include <filesystem>

#include "utils.h"
#include "vector3d.h"

int main() {
  constexpr int kImageWidth = 256;
  constexpr int kImageHeight = 256;

  std::vector<std::vector<Color>> image(kImageHeight, std::vector<Color>(kImageWidth));

  for (int row = kImageHeight - 1; row >= 0; --row) {
    for (int column = 0; column < kImageWidth; ++column) {
      Color color(
          static_cast<double>(column) / (kImageWidth - 1),
          static_cast<double>(row) / (kImageHeight - 1),
          0.25
      );
      image[row][column] = color;
    }
  }

  utils::WritePNG(std::filesystem::current_path().append("image.png"), kImageWidth, kImageHeight, 1, image);
}