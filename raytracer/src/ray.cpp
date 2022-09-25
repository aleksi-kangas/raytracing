#include "ray.h"

namespace rt {
Ray::Ray(glm::vec3 origin, glm::vec3 direction) : origin_(origin), direction_(direction) {}

glm::vec3 Ray::Origin() const { return origin_; }

glm::vec3 Ray::Direction() const { return direction_; }

glm::vec3 Ray::At(float t) const {
  return origin_ + t * direction_;
}

}  // namespace rt
