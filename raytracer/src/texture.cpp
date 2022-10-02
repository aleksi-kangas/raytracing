#include "texture.h"

#include <algorithm>

#include "utils.h"

namespace rt {

SolidColorTexture::SolidColorTexture(glm::vec3 color) : color_{color} {}

SolidColorTexture::SolidColorTexture(float red, float green, float blue) : color_{red, green, blue} {}

glm::vec3 SolidColorTexture::Sample(float u, float v, const glm::vec3& point) const {
  return color_;
}

NoiseTexture::NoiseTexture(float scale) : scale_{scale} {}

glm::vec3 NoiseTexture::Sample(float u, float v, const glm::vec3& point) const {
  return glm::vec3{1, 1, 1} * 0.5f * (1.0f + sinf(scale_ * point.z + 10.0f * perlin_.Turbulence(scale_ * point)));
}

ImageTexture::ImageTexture(const std::filesystem::path& path) : buffer_{utils::png::Decode(path, width_, height_)} {}

glm::vec3 ImageTexture::Sample(float u, float v, const glm::vec3& point) const {
  u = std::clamp(u, 0.0f, 1.0f);
  v = 1.0f - std::clamp(v, 0.0f, 1.0f);
  auto column = static_cast<int32_t>(u * static_cast<float>(width_));
  column = std::clamp(column, 0, width_ - 1);
  auto row = static_cast<int32_t>(v * static_cast<float>(height_));
  row = std::clamp(row, 0, height_ - 1);
  const uint32_t pixel_index = row * width_ * 3 + column * 3;
  const uint8_t red = buffer_[pixel_index + 0];
  const uint8_t green = buffer_[pixel_index + 1];
  const uint8_t blue = buffer_[pixel_index + 2];
  constexpr float color_scale = 1.0f / 255.0f;
  return glm::vec3{red, green, blue} * color_scale;
}

}  // namespace rt
