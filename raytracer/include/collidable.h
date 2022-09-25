#pragma once

#include "collision.h"
#include "ray.h"

namespace rt {
template<class T>
class Collidable {
 public:
  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
    return static_cast<const T*>(this)->Collide(ray, t_min, t_max, collision);
  }
};

}  // namespace rt
