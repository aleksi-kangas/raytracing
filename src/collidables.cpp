#include "collidables.h"

#include <algorithm>

void Collidables::AddCollidable(const std::shared_ptr<Collidable> &object) {
  objects_.emplace_back(object);
}

void Collidables::Clear() {
  objects_.clear();
}

bool Collidables::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  bool collided = false;
  double closest_so_far = t_max;
  std::for_each(objects_.begin(), objects_.end(), [&](const auto &object) {
    if (object->Collide(ray, t_min, closest_so_far, collision)) {
      collided = true;
      closest_so_far = collision.t;
    }
  });
  return collided;
}
