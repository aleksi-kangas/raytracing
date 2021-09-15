#include "camera.h"

#include <cmath>

#include "utils.h"

Camera::Camera(const Point3D &position,
               const Point3D &target,
               double aspect_ratio,
               double vertical_fov_degrees,
               double aperture,
               double focus_distance,
               Vector3D view_up_direction)
    : position_(position), lens_radius_(aperture / 2.0) {
  double theta = utils::DegreesToRadians(vertical_fov_degrees);
  double h = tan(theta / 2);
  double viewport_height = 2.0 * h;
  double viewport_width = aspect_ratio * viewport_height;

  w_ = Vector3D::UnitVector(position - target);
  u_ = Vector3D::UnitVector(Vector3D::CrossProduct(view_up_direction, w_));
  v_ = Vector3D::CrossProduct(w_, u_);

  horizontal_ = viewport_width * u_ * focus_distance;
  vertical_ = viewport_height * v_ * focus_distance;
  lower_left_corner_ = position_ - horizontal_ / 2 - vertical_ / 2 - w_ * focus_distance;
}

Ray Camera::GetRay(double u, double v) const {
  Vector3D random_direction = lens_radius_ * Vector3D::RandomInUnitDisk();
  Vector3D offset = u_ * random_direction.X() + v_ * random_direction.Y();
  return {
      position_ + offset,
      lower_left_corner_ + u * horizontal_ + v * vertical_ - position_ - offset
  };
}
