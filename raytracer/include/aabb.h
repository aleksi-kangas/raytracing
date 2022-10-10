#pragma once

#include <cstdint>
#include <limits>

#include "glm/glm.hpp"

#include "ray.h"

namespace rt {
/**
 * Axis-Aligned Bounding Box
 */
class AABB {
 public:
  AABB() = default;
  AABB(glm::vec3 min_point, glm::vec3 max_point);

  [[nodiscard]] glm::vec3 MinPoint() const;
  [[nodiscard]] glm::vec3 MaxPoint() const;

  [[nodiscard]] bool Collide(const Ray& ray, float t_min, float t_max) const;

  [[nodiscard]] float Area() const;
  [[nodiscard]] glm::vec3 Centroid() const;
  [[nodiscard]] int32_t LongestAxis() const;

  static AABB SurroundingBox(const AABB& box0, const AABB& box1);

 private:
  glm::vec3 min_point_{std::numeric_limits<float>::max()};
  glm::vec3 max_point_{std::numeric_limits<float>::lowest()};
};
}  // namespace rt
