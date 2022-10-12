#pragma once

#include <variant>
#include <vector>

#include "collision.h"
#include "crtp.h"
#include "ray.h"

namespace rt {
class Box;
class MovingSphere;
class RectangleXY;
class RectangleXZ;
class RectangleYZ;
class Sphere;
using primitive_t = std::variant<Box,
                                 MovingSphere,
                                 RectangleXY,
                                 RectangleXZ,
                                 RectangleYZ,
                                 Sphere>;
class ConstantMedium;
class Flip;
using rotatable_translatable_primitive_t = std::variant<Box,
                                                        ConstantMedium,
                                                        Flip,
                                                        MovingSphere,
                                                        RectangleXY,
                                                        RectangleXZ,
                                                        RectangleYZ,
                                                        Sphere>;

class RotateTranslate;
using collidable_t = std::variant<
    Box,
    ConstantMedium,
    Flip,
    MovingSphere,
    RectangleXY,
    RectangleXZ,
    RectangleYZ,
    Sphere,
    RotateTranslate
>;
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

  [[nodiscard]] float PDFValue(const glm::vec3& origin, const glm::vec3& direction) const {
    return this->Actual().PDFValue(origin, direction);
  }

  [[nodiscard]] glm::vec3 RandomTowards(const glm::vec3& origin) const {
    return this->Actual().RandomTowards(origin);
  }

 private:
  Collidable() = default;
  friend T;
};
}  // namespace rt
