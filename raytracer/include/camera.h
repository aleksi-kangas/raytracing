#pragma once

#include "glm/glm.hpp"

#include "ray.h"

namespace rt {
class Camera {
 public:
  explicit Camera(glm::vec3 origin);

  [[nodiscard]] Ray ShootRay(const glm::vec2& coordinate) const;

 private:
  glm::vec3 origin_;
};
}  // namespace rt
