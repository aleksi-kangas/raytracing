#pragma once

#include "collision.h"
#include "ray.h"

namespace rt {
template<class T>
class Collidable {
 public:
  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
    return static_cast<const T*>(this)->CollideImpl(ray, t_min, t_max, collision);
  }

 private:
  Collidable() = default;
  friend T;
};

}  // namespace rt
