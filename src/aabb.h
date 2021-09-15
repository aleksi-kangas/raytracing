#pragma once

#include "ray.h"
#include "vector3d.h"

class AxisAlignedBoundingBox {
 public:
  AxisAlignedBoundingBox() = default;
  AxisAlignedBoundingBox(const Point3D &min_point, const Point3D &max_point);

  /**
   * Get minimum point of the AABB.
   * @return minimum point in terms of coordinates
   */
  [[nodiscard]] Point3D MinPoint() const;

  /**
   * Get maximum point of the AABB.
   * @return maximum point in terms of coordinates
   */
  [[nodiscard]] Point3D MaxPoint() const;

  /**
   * Compute collision of a ray and the AABB.
   * @param ray inbound ray
   * @param t_min threshold minimum
   * @param t_max threshold maximum
   * @return true if collision happened, false otherwise
   */
  [[nodiscard]] bool Collide(const Ray &ray, double t_min, double t_max) const;

  /**
   * Compute a minimum AABB that surrounds the given AABBs.
   * @param box0 first AABB
   * @param box1 second AABB
   * @return AABB that surrounds box0 and box 1
   */
  static AxisAlignedBoundingBox SurroundingBox(const AxisAlignedBoundingBox &box0, const AxisAlignedBoundingBox &box1);

 private:
  Point3D min_point_, max_point_;
};
