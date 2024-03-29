#pragma once

#include "glm/glm.hpp"

#include "aabb.h"
#include "collidable.h"
#include "collision.h"
#include "material.h"
#include "ray.h"

namespace rt {
class Sphere : public Collidable<Sphere> {
 public:
  Sphere(glm::vec3 center, float radius, material_t material);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

  [[nodiscard]] float PDFValue(const glm::vec3& origin, const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 RandomTowards(const glm::vec3& origin) const;

 private:
  glm::vec3 center_;
  float radius_;
  material_t material_;

  static void ComputeUV(const glm::vec3& point, float& u, float& v);
};

class MovingSphere : public Collidable<MovingSphere> {
 public:
  MovingSphere(glm::vec3 center0, glm::vec3 center1, float time0, float time1, float radius, material_t material);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

  [[nodiscard]] float PDFValue(const glm::vec3& origin, const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 RandomTowards(const glm::vec3& origin) const;

  [[nodiscard]] glm::vec3 CenterAt(float time) const;

 private:
  glm::vec3 center0_, center1_;
  float time0_, time1_;
  float radius_;
  material_t material_;
};
}  // namespace rt
