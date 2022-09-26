#pragma once

#include "glm/glm.hpp"

#include "ray.h"

namespace rt {
class Camera {
 public:
  Camera(glm::vec3 origin, glm::vec3 target, glm::vec3 view_up_direction, float vertical_fov, float aspect_ratio);

  [[nodiscard]] Ray ShootRay(const glm::vec2& coordinate) const;

 private:
  glm::vec3 origin_;
  glm::vec3 lower_left_corner_;
  glm::vec3 horizontal_;
  glm::vec3 vertical_;
};
}  // namespace rt
