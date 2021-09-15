#include "collidables.h"

#include <algorithm>

void Collidables::AddCollidable(const std::shared_ptr<Collidable> &object) {
  objects_.emplace_back(object);
}

void Collidables::Clear() {
  objects_.clear();
}

std::vector<std::shared_ptr<Collidable>> Collidables::CollidableObjects() const {
  return objects_;
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

bool Collidables::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  if (objects_.empty()) {
    return false;
  }

  AxisAlignedBoundingBox temp_box;
  bool first_box = true;

  for (const auto &object: objects_) {
    if (!object->BoundingBox(time0, time1, temp_box)) {
      return false;
    }
    bounding_box = first_box ? temp_box : AxisAlignedBoundingBox::SurroundingBox(bounding_box, temp_box);
    first_box = false;
  }
  return true;
}
