#include "translate.h"

#include <utility>

Translate::Translate(std::shared_ptr<Collidable> object, const Vector3D &translation)
    : object_(std::move(object)), translation_(translation) {}

bool Translate::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  Ray translated_ray(ray.Origin() - translation_, ray.Direction(), ray.Time());
  if (!object_->Collide(translated_ray, t_min, t_max, collision)) {
    return false;
  }
  collision.point += translation_;
  collision.SetFaceNormal(translated_ray, collision.normal);
  return true;
}

bool Translate::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  if (!object_->BoundingBox(time0, time1, bounding_box)) {
    return false;
  }
  bounding_box = AxisAlignedBoundingBox(
      bounding_box.MinPoint() + translation_,
      bounding_box.MaxPoint() + translation_
  );
  return true;
}
