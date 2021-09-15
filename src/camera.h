#pragma once

#include "ray.h"
#include "vector3d.h"

/**
 * Represents the origin of rays and works as a viewpoint in 3D space.
 */
class Camera {
 public:
  Camera(const Point3D &position,
         const Point3D &target,
         double aspect_ratio,
         double vertical_fov_degrees,
         double aperture,
         double focus_distance,
         Vector3D view_up_direction = Vector3D(0, 1, 0));

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
  Vector3D horizontal_, vertical_;
  Vector3D v_, u_, w_;
  double lens_radius_;
};
