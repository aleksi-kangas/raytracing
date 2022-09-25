#pragma once

#include "glm/glm.hpp"

namespace rt {
class Ray {
 public:
  Ray(glm::vec3 origin, glm::vec3 direction);

  [[nodiscard]] glm::vec3 Origin() const;

  [[nodiscard]] glm::vec3 Direction() const;

  [[nodiscard]] glm::vec3 At(float t) const;

 private:
  glm::vec3 origin_;
  glm::vec3 direction_;
};
}  // namespace rt
