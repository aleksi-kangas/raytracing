#pragma once

#include "glm/glm.hpp"

#include "aabb.h"
#include "collidable.h"
#include "collision.h"
#include "ray.h"
#include "transformables.h"

namespace rt {
class Transform : public Collidable<Transform> {
 public:
  explicit Transform(transformable_t collidable,
                     float rotate_y = 0.0f,
                     glm::vec3 translate = {0.0f, 0.0f, 0.0f});

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

  [[nodiscard]] float PDFValue(const glm::vec3& origin, const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 RandomTowards(const glm::vec3& origin) const;

 private:
  transformable_t collidable_;
  // TODO Support rotation around other axes.
  float rotate_y_;
  glm::vec3 translate_{0.0f, 0.0f, 0.0f};

  [[nodiscard]] glm::mat4 TransformationMatrix() const;
  [[nodiscard]] glm::mat4 InverseTransformationMatrix() const;
};
}  // namespace rt
