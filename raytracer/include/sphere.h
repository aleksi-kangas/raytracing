#pragma once

#include "glm/glm.hpp"

#include "collidable.h"
#include "collision.h"
#include "material.h"
#include "ray.h"

namespace rt {
class Sphere : public Collidable<Sphere> {
 public:
  Sphere(glm::vec3 center, float radius, const Material* material);

  bool CollideImpl(const Ray& ray, float t_min, float t_max, Collision& collision) const;

 private:
  glm::vec3 center_;
  float radius_;
  const Material* material_;
};
}  // namespace rt
