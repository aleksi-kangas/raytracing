#pragma once

#include "glm/glm.hpp"

#include "box.h"
#include "collidable.h"
#include "rectangle.h"
#include "sphere.h"

namespace rt {
class RotateTranslate : public Collidable<RotateTranslate> {
 public:
  explicit RotateTranslate(translatable_rotatable_collidable_t collidable,
                           float rotate_y = 0.0f,
                           glm::vec3 translate = {0.0f, 0.0f, 0.0f});

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

 private:
  translatable_rotatable_collidable_t collidable_;
  // TODO Support rotation around other axes.
  float rotate_y_;
  glm::vec3 translate_{0.0f, 0.0f, 0.0f};

  [[nodiscard]] glm::mat4 TransformationMatrix() const;
  [[nodiscard]] glm::mat4 InverseTransformationMatrix() const;
};
}  // namespace rt
