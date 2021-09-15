#include "camera.h"

Camera::Camera(const Point3D &position, double aspect_ratio, double viewport_height, double focal_length)
    : position_(position) {
  double viewport_width = aspect_ratio * viewport_height;
  horizontal_ = {viewport_width, 0, 0};
  vertical_ = {0, viewport_height, 0};
  lower_left_corner_ = position_ - horizontal_ / 2 - vertical_ / 2 - Vector3D(0, 0, focal_length);
}

Ray Camera::GetRay(double u, double v) const {
  return {position_, lower_left_corner_ + u * horizontal_ + v * vertical_ - position_};
}
