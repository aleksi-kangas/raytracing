#pragma once

#include <memory>

#include "aabb.h"
#include "collidable.h"
#include "material.h"
#include "ray.h"
#include "vector3d.h"

class MovingSphere : public Collidable {
 public:
  MovingSphere(const Point3D &center0,
               const Point3D &center1,
               double time0,
               double time1,
               double radius,
               std::shared_ptr<Material> material);

  /**
   * Get the center of the moving sphere at the given time.
   * @param time center at this time
   * @return center at the given time
   */
  [[nodiscard]] Point3D CenterAt(double time) const;

  /**
   * Compute collision of a ray and the moving sphere object.
   * @param[in] ray inbound ray
   * @param[in] t_min minimum threshold
   * @param[in] t_max maximum threshold
   * @param[out] collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

  /**
   * Compute a minimum AABB that surrounds the moving sphere object.
   * @param[in] time0 minimum threshold
   * @param[in] time1 maximum threshold
   * @param[out] bounding_box receives the computed AABB
   * @return true if an AABB exists, false otherwise
   */
  bool BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const override;

 private:
  Point3D center0_, center1_;
  double time0_, time1_;
  double radius_;
  std::shared_ptr<Material> material_;
};
