#include "camera.h"

namespace rt {
Camera::Camera(glm::vec3 origin, glm::vec3 target, glm::vec3 view_up_direction, float vertical_fov, float aspect_ratio)
    : origin_{origin} {
  const float theta = glm::radians(vertical_fov);
  const float h = glm::tan(theta / 2.0f);
  const float viewport_height = 2.0f * h;
  const float viewport_width = aspect_ratio * viewport_height;

  const glm::vec3 w = glm::normalize(origin - target);
  const glm::vec3 u = glm::normalize(glm::cross(view_up_direction, w));
  const glm::vec3 v = glm::cross(w, u);

  horizontal_ = viewport_width * u;
  vertical_ = viewport_height * v;
  lower_left_corner_ = origin - horizontal_ / 2.0f - vertical_ / 2.0f - w;
}

Ray Camera::ShootRay(const glm::vec2& coordinate) const {
  return {origin_, lower_left_corner_ + coordinate.x * horizontal_ + coordinate.y * vertical_ - origin_};
}

}  // namespace rt
