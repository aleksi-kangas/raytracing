#pragma once

#include "glm/glm.hpp"

#include "collidable.h"
#include "collision.h"
#include "ray.h"

namespace rt {
class Sphere : public Collidable<Sphere> {
 public:
  Sphere(glm::vec3 center, float radius);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

 private:
  glm::vec3 center_;
  float radius_;
};
}  // namespace rt
