#include "aabb.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

#define OPTIMIZED_AABB_COLLIDE 1

namespace rt {
AABB::AABB(glm::vec3 min_point, glm::vec3 max_point) : min_point_{min_point}, max_point_{max_point} {}

glm::vec3 AABB::MinPoint() const { return min_point_; }

glm::vec3 AABB::MaxPoint() const { return max_point_; }

bool AABB::Collide(const Ray& ray, float t_min, float t_max) const {
  for (int32_t axis = 0; axis < 3; ++axis) {
#if OPTIMIZED_AABB_COLLIDE
    const float inverse_direction = 1.0f / ray.Direction()[axis];
    float t0 = (min_point_[axis] - ray.Origin()[axis]) * inverse_direction;
    float t1 = (max_point_[axis] - ray.Origin()[axis]) * inverse_direction;
    if (inverse_direction < 0.0f) {
      std::swap(t0, t1);
    }
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min) return false;
#else
    const float t0 = fmin((min_point_[axis] - ray.Origin()[axis]) / ray.Direction()[axis],
                          (max_point_[axis] - ray.Origin()[axis]) / ray.Direction()[axis]);
    const float t1 = fmax((min_point_[axis] - ray.Origin()[axis]) / ray.Direction()[axis],
                          (max_point_[axis] - ray.Origin()[axis]) / ray.Direction()[axis]);
    t_min = fmax(t_min, t0);
    t_max = fmin(t_max, t1);
    if (t_max <= t_min) return false;
#endif
  }
  return true;
}

float AABB::Area() const {
  constexpr glm::vec3 kNegativeInfinity{std::numeric_limits<float>::lowest()};
  constexpr glm::vec3 kPositiveInfinity{std::numeric_limits<float>::max()};
  if (min_point_ == kPositiveInfinity || max_point_ == kNegativeInfinity) return 0.0f;
  const glm::vec3 extent = max_point_ - min_point_;
  return 2.0f * (extent.x * extent.y + extent.y * extent.z + extent.z * extent.x);
}

AABB AABB::SurroundingBox(const AABB& box0, const AABB& box1) {
  const glm::vec3 min_point{glm::min(box0.min_point_, box1.min_point_)};
  const glm::vec3 max_point{glm::max(box0.max_point_, box1.max_point_)};
  return {min_point, max_point};
}

}  // namespace rt
