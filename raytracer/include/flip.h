#pragma once

#include "box.h"
#include "collidable.h"
#include "primitives.h"
#include "rectangle.h"
#include "sphere.h"

namespace rt {
class Flip : public Collidable<Flip> {
 public:
  explicit Flip(primitive_t primitive);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

  [[nodiscard]] float PDFValue(const glm::vec3& origin, const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 RandomTowards(const glm::vec3& origin) const;

 private:
  primitive_t primitive_;
};
}  // namespace rt
