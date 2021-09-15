#include <filesystem>

#include "renderer.h"
#include "scene.h"
#include "utils.h"
#include "vector3d.h"

int main() {
  constexpr double kAspectRatio = 16.0 / 9.0;
  constexpr int kImageWidth = 400;
  constexpr int kImageHeight = static_cast<int>(kImageWidth / kAspectRatio);
  constexpr int kSamplesPerPixel = 1;

  Scene scene(kImageWidth, kImageHeight, kSamplesPerPixel);
  Renderer renderer(scene);
  const std::vector<std::vector<Color>> image = renderer.Render();

  utils::WritePNG(std::filesystem::current_path().append("image.png"),
                  kImageWidth,
                  kImageHeight,
                  kSamplesPerPixel,
                  image);
}