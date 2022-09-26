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

class MovingSphere : public Collidable<MovingSphere> {
 public:
  MovingSphere(glm::vec3 center0, glm::vec3 center1, float time0, float time1, float radius, const Material* material);

  bool CollideImpl(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  [[nodiscard]] glm::vec3 CenterAt(float time) const;

 private:
  glm::vec3 center0_, center1_;
  float time0_, time1_;
  float radius_;
  const Material* material_;
};
}  // namespace rt
