#pragma once

#include "glm/glm.hpp"

namespace rt {
class Ray {
 public:
  Ray() = default;
  Ray(glm::vec3 origin, glm::vec3 direction, float time = 0.0f);

  [[nodiscard]] glm::vec3 Origin() const;

  [[nodiscard]] glm::vec3 Direction() const;

  [[nodiscard]] float Time() const;

  [[nodiscard]] glm::vec3 At(float t) const;

 private:
  glm::vec3 origin_{0, 0, 0};
  glm::vec3 direction_{0, 0, 0};
  float time_ = 0.0f;
};
}  // namespace rt
