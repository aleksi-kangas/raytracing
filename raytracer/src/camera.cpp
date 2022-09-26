#include "camera.h"

#include "random.h"

namespace rt {
Camera::Camera(glm::vec3 origin,
               glm::vec3 target,
               glm::vec3 view_up_direction,
               float vertical_fov,
               float aspect_ratio,
               float aperture,
               float focus_distance,
               float time0,
               float time1)
    : origin_{origin}, lens_radius_(aperture / 2.0f), time0_{time0}, time1_{time1} {
  const float theta = glm::radians(vertical_fov);
  const float h = glm::tan(theta / 2.0f);
  const float viewport_height = 2.0f * h;
  const float viewport_width = aspect_ratio * viewport_height;

  w_ = glm::normalize(origin - target);
  u_ = glm::normalize(glm::cross(view_up_direction, w_));
  v_ = glm::cross(w_, u_);

  horizontal_ = focus_distance * viewport_width * u_;
  vertical_ = focus_distance * viewport_height * v_;
  lower_left_corner_ = origin - horizontal_ / 2.0f - vertical_ / 2.0f - focus_distance * w_;
}

Ray Camera::ShootRay(const glm::vec2& coordinate) const {
  const glm::vec3 random_in_lens = lens_radius_ * random::InUnitDisk();
  const glm::vec3 offset = u_ * random_in_lens.x + v_ * random_in_lens.y;
  return {origin_ + offset,
          lower_left_corner_ + coordinate.x * horizontal_ + coordinate.y * vertical_ - origin_ - offset,
          random::Float(time0_, time1_)};
}

}  // namespace rt
