#pragma once

#include <memory>

#include "aabb.h"
#include "collidable.h"
#include "ray.h"
#include "vector3d.h"

class Sphere : public Collidable {
 public:
  Sphere(const Point3D &center, double radius, std::shared_ptr<Material> material);

  /**
   * Compute collision of a ray and the sphere object.
   * @param[in] ray inbound ray
   * @param[in] t_min minimum threshold
   * @param[in] t_max maximum threshold
   * @param[out] collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

  /**
   * Compute a minimum AABB that surrounds the sphere object.
   * @param[in] time0 minimum threshold
   * @param[in] time1 maximum threshold
   * @param[out] bounding_box receives the computed AABB
   * @return true if an AABB exists, false otherwise
   */
  bool BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const override;

 private:
  Point3D center_;
  double radius_;
  std::shared_ptr<Material> material_;
};
