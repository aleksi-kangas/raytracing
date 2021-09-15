#include "flip_face.h"

#include <utility>

FlipFace::FlipFace(std::shared_ptr<Collidable> object) : object_(std::move(object)) {}

bool FlipFace::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  if (!object_->Collide(ray, t_min, t_max, collision)) {
    return false;
  }

  collision.is_front_face = !collision.is_front_face;
  return true;
}

bool FlipFace::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  return object_->BoundingBox(time0, time1, bounding_box);
}
