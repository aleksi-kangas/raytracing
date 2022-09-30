#include "ray.h"

namespace rt {
Ray::Ray(glm::vec3 origin, glm::vec3 direction, float time)
    : origin_{origin}, direction_{direction}, inverse_direction_{1.0f / direction}, time_{time} {}

glm::vec3 Ray::Origin() const { return origin_; }

glm::vec3 Ray::Direction() const { return direction_; }

glm::vec3 Ray::InverseDirection() const { return inverse_direction_; }

float Ray::Time() const { return time_; }

glm::vec3 Ray::At(float t) const {
  return origin_ + t * direction_;
}

}  // namespace rt
