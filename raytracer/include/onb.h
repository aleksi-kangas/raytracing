#pragma once

#include "glm/glm.hpp"

namespace rt {
struct ONB {
  explicit ONB(const glm::vec3& w);

  [[nodiscard]] glm::vec3 Local(float a, float b, float c) const;
  [[nodiscard]] glm::vec3 Local(const glm::vec3& a) const;

  glm::vec3 u{1, 0, 0};
  glm::vec3 v{0, 1, 0};
  glm::vec3 w{0, 0, 1};
};
}  // namespace rt
