#pragma once

#include <variant>
#include <vector>

#include "collision.h"
#include "crtp.h"
#include "ray.h"

namespace rt {
class Sphere;
class MovingSphere;
class RectangleXY;
class RectangleXZ;
class RectangleYZ;
using collidable_t = std::variant<Sphere, MovingSphere, RectangleXY, RectangleXZ, RectangleYZ>;
using collidable_container_t = std::vector<collidable_t>;

class AABB;

template<class T>
class Collidable : public CRTP<Collidable<T>> {
 public:
  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
    return this->Actual().Collide(ray, t_min, t_max, collision);
  }

  bool BoundingBox(float time0, float time1, AABB& bounding_box) const {
    return this->Actual().BoundingBox(time0, time1, bounding_box);
  }

  [[nodiscard]] glm::vec3 Centroid() const {
    return this->Actual().Centroid();
  }

 private:
  Collidable() = default;
  friend T;
};
}  // namespace rt
