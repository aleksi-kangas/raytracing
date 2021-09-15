#include "ray.h"

Ray::Ray(const Point3D &origin, const Vector3D &direction)
    : origin_(origin), direction_(direction) {}

Point3D Ray::Origin() const {
  return origin_;
}

Vector3D Ray::Direction() const {
  return direction_;
}

Point3D Ray::PointAt(double time) const {
  return origin_ + time * direction_;
}
