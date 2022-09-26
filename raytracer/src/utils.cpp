#include "utils.h"

#include <algorithm>

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

}  // namespace rt::utils
