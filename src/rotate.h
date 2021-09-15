#pragma once

#include <memory>

#include "aabb.h"
#include "collidable.h"
#include "material.h"
#include "ray.h"
#include "vector3d.h"

class RotateY : public Collidable {
 public:
  RotateY(std::shared_ptr<Collidable> object, double angle_degrees);

  /**
   * Compute collision of a ray and the rotated object.
   * @param[in] ray inbound ray
   * @param[in] t_min minimum threshold
   * @param[in] t_max maximum threshold
   * @param[out] collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

  /**
   * Compute a minimum AABB that surrounds the rotated object.
   * @param[in] time0 minimum threshold
   * @param[in] time1 maximum threshold
   * @param[out] bounding_box receives the computed AABB
   * @return true if an AABB exists, false otherwise
   */
  bool BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const override;

 private:
  std::shared_ptr<Collidable> object_;
  double sin_theta_, cos_theta_;
  bool has_box_;
  AxisAlignedBoundingBox bounding_box_;
};
