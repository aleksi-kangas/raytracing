#include "camera.h"

#include <cmath>

#include "utils.h"

Camera::Camera(const Point3D &position,
               const Point3D &target,
               double aspect_ratio,
               double vertical_fov_degrees,
               Vector3D view_up_direction)
    : position_(position) {
  double theta = utils::DegreesToRadians(vertical_fov_degrees);
  double h = tan(theta / 2);
  double viewport_height = 2.0 * h;
  double viewport_width = aspect_ratio * viewport_height;

  Vector3D w = Vector3D::UnitVector(position - target);
  Vector3D u = Vector3D::UnitVector(Vector3D::CrossProduct(view_up_direction, w));
  Vector3D v = Vector3D::CrossProduct(w, u);

  horizontal_ = viewport_width * u;
  vertical_ = viewport_height * v;
  lower_left_corner_ = position_ - horizontal_ / 2 - vertical_ / 2 - w;
}

Ray Camera::GetRay(double u, double v) const {
  return {position_, lower_left_corner_ + u * horizontal_ + v * vertical_ - position_};
}
