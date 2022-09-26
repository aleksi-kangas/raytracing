#include "utils.h"

namespace rt::utils {
uint32_t ColorToRGBA(const glm::vec4& color) {
  const auto r = static_cast<uint8_t>(color.r * 255.0f);
  const auto g = static_cast<uint8_t>(color.g * 255.0f);
  const auto b = static_cast<uint8_t>(color.b * 255.0f);
  const auto a = static_cast<uint8_t>(color.a * 255.0f);
  return (a << 24) | (b << 16) | (g << 8) | (r << 0);
}

}  // namespace rt::utils
