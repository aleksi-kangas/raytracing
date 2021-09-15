#include "rotate.h"

#include <utility>

#include "utils.h"

RotateY::RotateY(std::shared_ptr<Collidable> object, double angle_degrees)
    : object_(std::move(object)),
      sin_theta_(sin(utils::DegreesToRadians(angle_degrees))),
      cos_theta_(cos(utils::DegreesToRadians(angle_degrees))) {
  has_box_ = object_->BoundingBox(0.0, 1.0, bounding_box_);
  Point3D min_point(utils::kInfinity, utils::kInfinity, utils::kInfinity);
  Point3D max_point(utils::kNegativeInfinity, utils::kNegativeInfinity, utils::kNegativeInfinity);

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 2; ++k) {
        double x = i * bounding_box_.MaxPoint().X() + (1.0 - i) * bounding_box_.MinPoint().X();
        double y = j * bounding_box_.MaxPoint().Y() + (1.0 - j) * bounding_box_.MinPoint().Y();
        double z = k * bounding_box_.MaxPoint().Z() + (1.0 - k) * bounding_box_.MinPoint().Z();

        double rotated_x = cos_theta_ * x + sin_theta_ * z;
        double rotated_z = -sin_theta_ * x + cos_theta_ * z;

        Vector3D rotated(rotated_x, y, rotated_z);
        for (int axis = 0; axis < 3; ++axis) {
          min_point[axis] = std::min(min_point[axis], rotated[axis]);
          max_point[axis] = std::max(max_point[axis], rotated[axis]);
        }
      }
    }
  }
  bounding_box_ = AxisAlignedBoundingBox(min_point, max_point);
}

bool RotateY::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  Point3D origin = ray.Origin();
  origin[0] = cos_theta_ * ray.Origin()[0] - sin_theta_ * ray.Origin()[2];
  origin[2] = sin_theta_ * ray.Origin()[0] + cos_theta_ * ray.Origin()[2];

  Vector3D direction = ray.Direction();
  direction[0] = cos_theta_ * ray.Direction()[0] - sin_theta_ * ray.Direction()[2];
  direction[2] = sin_theta_ * ray.Direction()[0] + cos_theta_ * ray.Direction()[2];

  Ray rotated_ray(origin, direction, ray.Time());

  if (!object_->Collide(rotated_ray, t_min, t_max, collision)) {
    return false;
  }

  Point3D point = collision.point;
  point[0] = cos_theta_ * collision.point[0] + sin_theta_ * collision.point[2];
  point[2] = -sin_theta_ * collision.point[0] + cos_theta_ * collision.point[2];

  Vector3D normal = collision.normal;
  normal[0] = cos_theta_ * collision.normal[0] + sin_theta_ * collision.normal[2];
  normal[2] = -sin_theta_ * collision.normal[0] + cos_theta_ * collision.normal[2];

  collision.point = point;
  collision.SetFaceNormal(rotated_ray, normal);
  return true;
}

bool RotateY::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  bounding_box = bounding_box_;
  return has_box_;
}
