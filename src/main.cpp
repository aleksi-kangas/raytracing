#include <filesystem>

#include "renderer.h"
#include "scene.h"
#include "utils.h"
#include "vector3d.h"

int main() {
  constexpr double kAspectRatio = 3.0 / 2.0;
  constexpr int kImageWidth = 1200;
  constexpr int kImageHeight = static_cast<int>(kImageWidth / kAspectRatio);
  constexpr int kSamplesPerPixel = 500;

  Scene scene(kImageWidth, kImageHeight, kSamplesPerPixel);
  scene.InitializeCamera();
  scene.InitializeWorld();
  Renderer renderer(scene);
  const std::vector<std::vector<Color>> image = renderer.Render();

  utils::WritePNG(std::filesystem::current_path().append("image.png"),
                  kImageWidth,
                  kImageHeight,
                  kSamplesPerPixel,
                  image);
}