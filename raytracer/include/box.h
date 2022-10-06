#pragma once

#include <array>

#include "glm/glm.hpp"

#include "aabb.h"
#include "collidable.h"
#include "material.h"
#include "rectangle.h"

namespace rt {
class Box : public Collidable<Box> {
 public:
  Box(glm::vec3 min_point, glm::vec3 max_point, material_t material);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

 private:
  glm::vec3 min_point_;
  glm::vec3 max_point_;
  material_t material_;
  std::array<rectangle_t, 6> sides_;
};
}  // namespace rt
