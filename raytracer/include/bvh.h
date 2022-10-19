#pragma once

#include <cstdint>
#include <vector>

#include "aabb.h"
#include "collidables.h"
#include "collision.h"
#include "ray.h"

/**
 * Bounding Volume Hierarchy (BVH) implementation to the book's scenario using the following resources as reference:
 * - 'How to Build a BVH' blog series by Jacco Bikker: https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/
 *       - Copyright(c) Jacco Bikker https://github.com/jbikker
 * - Physically Based Rendering, 3rd Edition, Chapter 4.3, "Bounding Volume Hierarchies"
 *      - Copyright(c) 1998-2016 Matt Pharr, Greg Humphreys, and Wenzel Jakob.
 * This application is licensed under the MIT License.
 * All source code from the references are under the rights of their respective owners.
 */

namespace rt {
/**
 * Defines the strategy for splits when building a BVH-tree.
 */
enum BVHSplitStrategy { Middle, EqualCounts, SurfaceAreaHeuristic, SplitStrategyCount };

class BVH {
 public:
  BVH(BVHSplitStrategy split_strategy, collidable_container_t& primitives, float time0, float time1);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

 private:
  BVHSplitStrategy split_strategy_;
  collidable_container_t& primitives_;
  float time0_, time1_;

  struct BVHNode {
    BVHNode(uint32_t first_primitive_offset, uint32_t primitive_count, AABB bounding_box = {})
        : first_primitive_offset{first_primitive_offset},
          primitive_count{primitive_count},
          bounding_box{bounding_box} {}
    uint32_t first_primitive_offset = 0, primitive_count = 0;
    int32_t split_axis = 0;
    AABB bounding_box;
  };
  std::vector<BVHNode> nodes_;

  // Surface Area Heuristic (SAH) constants.
  static constexpr float kTraversalCost = 0.125f;  // 1/8
  static constexpr float kIntersectionCost = 1.0f;

  [[nodiscard]] float ComputeSurfaceAreaHeuristic(const BVHNode& node, int32_t split_axis, float split_position) const;

  void ComputeNodeAABB(BVHNode& node);

  void Partition(BVHNode& node);

  bool TraverseRecursive(const BVHNode& node, const Ray& ray, float t_min, float t_max, Collision& collision) const;
};
}  // namespace rt
