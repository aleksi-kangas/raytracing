#include "box.h"

#include "rectangle.h"

Box::Box(const Point3D &min_point, const Point3D &max_point, const std::shared_ptr<Material> &material)
    : min_point_(min_point), max_point_(max_point) {
  Collidables collidables;
  collidables.AddCollidable(std::make_shared<RectangleXY>(
      min_point.X(), max_point.X(), min_point.Y(), max_point.Y(), max_point.Z(), material));
  collidables.AddCollidable(std::make_shared<RectangleXY>(
      min_point.X(), max_point.X(), min_point.Y(), max_point.Y(), min_point.Z(), material));

  collidables.AddCollidable(std::make_shared<RectangleXZ>(
      min_point.X(), max_point.X(), min_point.Z(), max_point.Z(), max_point.Y(), material));
  collidables.AddCollidable(std::make_shared<RectangleXZ>(
      min_point.X(), max_point.X(), min_point.Z(), max_point.Z(), min_point.Y(), material));

  collidables.AddCollidable(std::make_shared<RectangleYZ>(
      min_point.Y(), max_point.Y(), min_point.Z(), max_point.Z(), max_point.X(), material));
  collidables.AddCollidable(std::make_shared<RectangleYZ>(
      min_point.Y(), max_point.Y(), min_point.Z(), max_point.Z(), min_point.X(), material));

  sides_ = std::make_unique<BoundingVolumeHierarchyNode>(collidables, 0.0, 1.0);
}

bool Box::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  return sides_->Collide(ray, t_min, t_max, collision);
}

bool Box::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  bounding_box = AxisAlignedBoundingBox(min_point_, max_point_);
  return true;
}
