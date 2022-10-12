#pragma once

#include "glm/glm.hpp"

#include "aabb.h"
#include "box.h"
#include "collidable.h"
#include "collision.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"

namespace rt {
class ConstantMedium : public Collidable<ConstantMedium> {
 public:
  ConstantMedium(primitive_t boundary, float density, texture_t texture);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

  [[nodiscard]] float PDFValue(const glm::vec3& origin, const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 RandomTowards(const glm::vec3& origin) const;

 private:
  primitive_t boundary_;
  float negative_inverse_density_ = 0.0f;
  material_t phase_function_;
};
}  // namespace rt
