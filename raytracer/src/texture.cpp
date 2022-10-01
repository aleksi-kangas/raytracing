#include "texture.h"

namespace rt {

SolidColor::SolidColor(glm::vec3 color) : color_{color} {}

SolidColor::SolidColor(float red, float green, float blue) : color_{red, green, blue} {}

glm::vec3 SolidColor::Sample(float u, float v, const glm::vec3& point) const {
  return color_;
}

Checker::Checker(const Texture* even, const Texture* odd) : even_{even}, odd_{odd} {}

glm::vec3 Checker::Sample(float u, float v, const glm::vec3& point) const {
  const float sines = sinf(10.0f * point.x) * sinf(10.0f * point.y) * sinf(10.0f * point.z);
  if (sines >= 0.0f) {
    return even_->Sample(u, v, point);
  } else {
    return odd_->Sample(u, v, point);
  }
}

Noise::Noise(float scale) : scale_{scale} {}

glm::vec3 Noise::Sample(float u, float v, const glm::vec3& point) const {
  return glm::vec3{1, 1, 1} * 0.5f * (1.0f + sinf(scale_ * point.z + 10.0f * perlin_.Turbulence(scale_ * point)));
}

}  // namespace rt
