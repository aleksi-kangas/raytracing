#include "ray.h"

Ray::Ray(const Point3D &origin, const Vector3D &direction, double time)
    : origin_(origin), direction_(direction), time_(time) {}

Point3D Ray::Origin() const {
  return origin_;
}

Vector3D Ray::Direction() const {
  return direction_;
}

double Ray::Time() const {
  return time_;
}

Point3D Ray::PointAt(double time) const {
  return origin_ + time * direction_;
}
