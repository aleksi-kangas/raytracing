#pragma once

#include "ray.h"
#include "vector3d.h"

/**
 * Represents the origin of rays and works as a viewpoint in 3D space.
 */
class Camera {
 public:
  Camera(const Point3D &position, double aspect_ratio, double viewport_height, double focal_length);

  /**
   * Shoot a ray to u and v coordinates of the view plane.
   * @param u horizontal coordinate
   * @param v vertical coordinate
   * @return ray towards the given coordinates
   */
  [[nodiscard]] Ray GetRay(double u, double v) const;

 private:
  Point3D position_;
  Point3D lower_left_corner_;
  Vector3D horizontal_;
  Vector3D vertical_;
};
