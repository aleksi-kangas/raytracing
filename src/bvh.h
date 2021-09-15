#pragma once

#include <memory>
#include <vector>

#include "aabb.h"
#include "collidable.h"
#include "collidables.h"
#include "ray.h"

class BoundingVolumeHierarchyNode : public Collidable {
 public:
  /**
   * Construct a tree consisting of BVH nodes.
   * Important:
   * Does not work, when collidables contains 0 or 1 objects (infinite recursion).
   * Arguably, BVH structure won't give much benefit with just a single object,
   * I don't bother to fix this. BVH works fine with 2 or more objects in collidables.
   * Optimizations by Johann Meyer: https://github.com/RayTracing/raytracing.github.io/issues/890
   * @param collidables collection of collidable objects (see important)
   * @param time0 threshold minimum
   * @param time1 threshold maximum
   */
  BoundingVolumeHierarchyNode(const Collidables &collidables, double time0, double time1);

  /**
   * Construct a tree consisting of BVH nodes.
   * Important:
   * Does not work, when collidable_objects contains 0 or 1 objects (infinite recursion).
   * Arguably, BVH structure won't give much benefit with just a single object,
   * I don't bother to fix this. BVH works fine with 2 or more objects in collidable_objects.
   * Optimizations by Johann Meyer: https://github.com/RayTracing/raytracing.github.io/issues/890
   * @param collidable_objects collection of collidable objects (see important)
   * @param start index of the first element to the tree
   * @param end index of the last element to the tree
   * @param time0 threshold minimum
   * @param time1 threshold maximum
   */
  BoundingVolumeHierarchyNode(const std::vector<std::shared_ptr<Collidable>> &collidable_objects,
                              int start,
                              int end,
                              double time0,
                              double time1);

  /**
   * Compute collision of a ray and the BVH object.
   * @param ray inbound ray
   * @param t_min minimum threshold
   * @param t_max maximum threshold
   * @param collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

  /**
   * Compute a minimum AABB that surrounds the BVH object.
   * @param time0 minimum threshold
   * @param time1 maximum threshold
   * @param bounding_box receives the computed AABB
   * @return true if an AABB exists, false otherwise
   */
  bool BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const override;

 private:
  std::shared_ptr<Collidable> left_, right_;
  AxisAlignedBoundingBox bounding_box_;

  static bool BoxCompare(const std::shared_ptr<Collidable> &a, const std::shared_ptr<Collidable> &b, int axis);

  static bool BoxCompareX(const std::shared_ptr<Collidable> &a, const std::shared_ptr<Collidable> &b);

  static bool BoxCompareY(const std::shared_ptr<Collidable> &a, const std::shared_ptr<Collidable> &b);

  static bool BoxCompareZ(const std::shared_ptr<Collidable> &a, const std::shared_ptr<Collidable> &b);
};
