#pragma once

#include <variant>

#include "glm/glm.hpp"

#include "aabb.h"
#include "collidable.h"
#include "collision.h"
#include "material.h"

namespace rt {
class RectangleXY;
class RectangleXZ;
class RectangleYZ;
using rectangle_t = std::variant<RectangleXY, RectangleXZ, RectangleYZ>;

class RectangleXY : public Collidable<RectangleXY> {
 public:
  RectangleXY(glm::vec2 x, glm::vec2 y, float z, material_t material);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

  [[nodiscard]] float PDFValue(const glm::vec3& origin, const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 RandomTowards(const glm::vec3& origin) const;

 private:
  glm::vec2 x_{0.0f, 1.0f}, y_{0.0f, 1.0f};
  float z_ = 0.0f;
  material_t material_;
};

class RectangleXZ : public Collidable<RectangleXZ> {
 public:
  RectangleXZ(glm::vec2 x, glm::vec2 z, float y, material_t material);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

  [[nodiscard]] float PDFValue(const glm::vec3& origin, const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 RandomTowards(const glm::vec3& origin) const;

 private:
  glm::vec2 x_{0.0f, 1.0f}, z_{0.0f, 1.0f};
  float y_ = 0.0f;
  material_t material_;
};

class RectangleYZ : public Collidable<RectangleYZ> {
 public:
  RectangleYZ(glm::vec2 y, glm::vec2 z, float x, material_t material);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const;

  [[nodiscard]] glm::vec3 Centroid() const;

  [[nodiscard]] float PDFValue(const glm::vec3& origin, const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 RandomTowards(const glm::vec3& origin) const;

 private:
  glm::vec2 y_{0.0f, 1.0f}, z_{0.0f, 1.0f};
  float x_ = 0.0f;
  material_t material_;
};
}  // namespace rt
