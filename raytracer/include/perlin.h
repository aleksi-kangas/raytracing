#pragma once

#include <array>
#include <cstdint>

#include "glm/glm.hpp"

namespace rt {
class Perlin {
 public:
  Perlin();

  [[nodiscard]] float Noise(const glm::vec3& point) const;

  [[nodiscard]] float Turbulence(glm::vec3 point, uint32_t depth = 7) const;

 private:
  static constexpr uint32_t kPointCount = 256;

  std::array<glm::vec3, kPointCount> random_vectors_{};
  std::array<uint32_t, kPointCount> permutation_x_{};
  std::array<uint32_t, kPointCount> permutation_y_{};
  std::array<uint32_t, kPointCount> permutation_z_{};

  static void Permute(std::array<uint32_t, kPointCount>& permutation);

  static float Interpolation(glm::vec3 c[2][2][2], glm::vec3 uvw);
};
}  // namespace rt
