#include "utils.h"

#include <algorithm>
#include <cassert>
#include <format>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace rt::utils {
uint32_t ColorToRGBA(const glm::vec4& color) {
  const auto r = static_cast<uint8_t>(256.0f * std::clamp(color.r, 0.0f, 0.999f));
  const auto g = static_cast<uint8_t>(256.0f * std::clamp(color.g, 0.0f, 0.999f));
  const auto b = static_cast<uint8_t>(256.0f * std::clamp(color.b, 0.0f, 0.999f));
  const auto a = static_cast<uint8_t>(256.0f * std::clamp(color.a, 0.0f, 0.999f));
  return (a << 24) | (b << 16) | (g << 8) | (r << 0);
}

bool IsNearZero(const glm::vec3& vec) {
  constexpr float kEpsilon = 1e-8f;
  return glm::all(glm::lessThan(glm::abs(vec), glm::vec3{kEpsilon}));
}

namespace png {
std::vector<uint8_t> Decode(const std::filesystem::path& path, int32_t& width, int32_t& height) {
  int32_t channels;
  const uint8_t* image_data = stbi_load(path.string().c_str(), &width, &height, &channels, 3);
  if (image_data == nullptr) {
    throw std::runtime_error{std::format("Failed to decode PNG: {}.", path.string())};
  }
  std::vector<uint8_t> buffer(image_data, image_data + width * height * channels);
  return buffer;
}

void Encode(const std::filesystem::path& path, const Image& image) {
  // Vertically flip the buffer data.
  std::vector<uint32_t> buffer;
  buffer.resize(image.Width() * image.Height());
  for (int32_t row = 0; row < image.Height(); ++row) {
    for (int32_t column = 0; column < image.Width(); ++column) {
      const int32_t originalIndex = row * image.Width() + column;
      const int32_t flippedIndex = (image.Height() - row - 1) * image.Width() + column;
      buffer[flippedIndex] = image.Buffer()[originalIndex];
    }
  }
  const int32_t result = stbi_write_png(path.string().c_str(),
                                        image.Width(),
                                        image.Height(),
                                        4,
                                        static_cast<void*>(buffer.data()),
                                        image.Width() * 4);
  assert(result != 0);
}

}  // namespace rt::utils::png

}  // namespace rt::utils
