#pragma once

#include <memory>

#include "aabb.h"
#include "bvh.h"
#include "collidable.h"
#include "material.h"
#include "ray.h"
#include "vector3d.h"

class Box : public Collidable {
 public:
  Box(const Point3D &min_point, const Point3D &max_point, const std::shared_ptr<Material> &material);

  /**
   * Compute collision of a ray and the box object.
   * @param ray inbound ray
   * @param t_min minimum threshold
   * @param t_max maximum threshold
   * @param collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

  /**
   * Compute a minimum AABB that surrounds the box object.
   * @param time0 minimum threshold
   * @param time1 maximum threshold
   * @param bounding_box receives the computed AABB
   * @return true if an AABB exists, false otherwise
   */
  bool BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const override;

 private:
  Point3D min_point_, max_point_;
  std::unique_ptr<BoundingVolumeHierarchyNode> sides_;
};
