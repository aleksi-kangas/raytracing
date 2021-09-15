#pragma once

#include "vector3d.h"

/**
 * Represents a light ray in 3D space.
 */
class Ray {
 public:
  Ray() = default;
  Ray(const Point3D &origin, const Vector3D &direction);

  Ray(Ray &&) = default;
  Ray &operator=(Ray &&) = default;
  Ray(const Ray &) = default;
  Ray &operator=(const Ray &) = default;

  /**
   * Get origin of the ray.
   * @return origin of the ray
   */
  [[nodiscard]] Point3D Origin() const;

  /**
   * Get direction of the ray.
   * @return direction of the ray
   */
  [[nodiscard]] Vector3D Direction() const;

  /**
   * Get a point along the ray at a specific time.
   * @param time specific time
   * @return point at the given time
   */
  [[nodiscard]] Point3D PointAt(double time) const;

 private:
  Point3D origin_;
  Vector3D direction_;
};
