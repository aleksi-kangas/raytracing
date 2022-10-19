#include "bvh.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <limits>
#include <variant>

#include "box.h"
#include "rectangle.h"
#include "transform.h"
#include "sphere.h"

namespace rt {

BVH::BVH(BVHSplitStrategy split_strategy, collidable_container_t& primitives, float time0, float time1)
    : primitives_{primitives}, split_strategy_{split_strategy}, time0_{time0}, time1_{time1} {
  const auto n = static_cast<uint32_t>(primitives_.size());
  nodes_.reserve(2 * n);
  BVHNode& root = nodes_.emplace_back(0, n);
  ComputeNodeAABB(root);
  Partition(root);
}

bool BVH::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  return TraverseRecursive(nodes_[0], ray, t_min, t_max, collision);
}

float BVH::ComputeSurfaceAreaHeuristic(const BVHNode& node, int32_t split_axis, float split_position) const {
  AABB bounding_box;
  AABB left_box, right_box;
  uint32_t left_count = 0, right_count = 0;
  for (uint32_t i = 0; i < node.primitive_count; ++i) {
    AABB primitive_bounding_box;
    std::visit([&](const auto& primitive) { primitive.BoundingBox(time0_, time1_, primitive_bounding_box); },
               primitives_[node.first_primitive_offset + i]);
    bounding_box = AABB::SurroundingBox(bounding_box, primitive_bounding_box);
    if (primitive_bounding_box.Centroid()[split_axis] < split_position) {
      left_box = AABB::SurroundingBox(left_box, primitive_bounding_box);
      ++left_count;
    } else {
      right_box = AABB::SurroundingBox(right_box, primitive_bounding_box);
      ++right_count;
    }
  }
  return kTraversalCost + kIntersectionCost
      * (static_cast<float>(left_count) * left_box.Area() + static_cast<float>(right_count) * right_box.Area())
      / bounding_box.Area();
}

void BVH::ComputeNodeAABB(BVHNode& node) {
  for (uint32_t i = 0; i < node.primitive_count; ++i) {
    AABB primitive_bounding_box;
    std::visit([&](const auto& primitive) { primitive.BoundingBox(time0_, time1_, primitive_bounding_box); },
               primitives_[node.first_primitive_offset + i]);
    node.bounding_box = AABB::SurroundingBox(node.bounding_box, primitive_bounding_box);
  }
}

void BVH::Partition(BVHNode& node) {
  int32_t best_axis = node.bounding_box.LongestAxis();
  float best_split_position = 0.0f;
  const auto begin = primitives_.begin() + node.first_primitive_offset;
  const auto end = begin + node.primitive_count;
  auto partition_middle = begin + (end - begin) / 2;
  switch (split_strategy_) {
    case BVHSplitStrategy::Middle: {
      if (node.primitive_count == 1) return;
      best_split_position = node.bounding_box.Centroid()[best_axis];
      partition_middle = std::partition(
          begin, end,
          [&](const auto& primitive) {
            AABB primitive_bounding_box;
            std::visit([&](const auto& primitive) { primitive.BoundingBox(time0_, time1_, primitive_bounding_box); },
                       primitive);
            return primitive_bounding_box.Centroid()[best_axis] < best_split_position;
          });
      // If partitioning at the middle produced a reasonable result, we're done.
      // Otherwise, fallthrough to EqualCounts as a fallback partitioning method.
      if (begin != partition_middle && partition_middle != end) break;
    }
    case BVHSplitStrategy::EqualCounts: {
      if (node.primitive_count == 1) return;
      partition_middle = begin + (end - begin) / 2;
      std::nth_element(begin, partition_middle, end, [&](const auto& a, const auto& b) {
        AABB a_box, b_box;
        std::visit([&](const auto& primitive) { primitive.BoundingBox(time0_, time1_, a_box); }, a);
        std::visit([&](const auto& primitive) { primitive.BoundingBox(time0_, time1_, b_box); }, b);
        return a_box.Centroid()[best_axis] < b_box.Centroid()[best_axis];
      });
      break;
    }
    case BVHSplitStrategy::SurfaceAreaHeuristic: {
      float best_cost = std::numeric_limits<float>::max();
      for (int32_t axis = 0; axis < 3; ++axis) {
        for (uint32_t i = 0; i < node.primitive_count; ++i) {
          AABB primitive_bounding_box;
          std::visit([&](const auto& primitive) { primitive.BoundingBox(time0_, time1_, primitive_bounding_box); },
                     primitives_[node.first_primitive_offset + i]);
          const float candidate_position = primitive_bounding_box.Centroid()[axis];
          const float cost = ComputeSurfaceAreaHeuristic(node, axis, candidate_position);
          if (cost < best_cost) {
            best_axis = axis;
            best_split_position = candidate_position;
            best_cost = cost;
          }
        }
      }

      const float leaf_cost = kIntersectionCost * static_cast<float>(node.primitive_count);
      if (best_cost >= leaf_cost) {
        return;
      }
      partition_middle = std::partition(
          begin, end,
          [&](const auto& primitive) {
            AABB primitive_bounding_box;
            std::visit([&](const auto& primitive) { primitive.BoundingBox(time0_, time1_, primitive_bounding_box); },
                       primitive);
            return primitive_bounding_box.Centroid()[best_axis] < best_split_position;
          });
      break;
    }
    default: {
      assert(false);
    }
  }
  const uint32_t left_count = static_cast<uint32_t>(std::distance(begin, partition_middle));
  const uint32_t right_count = node.primitive_count - left_count;
  if (left_count == 0 || right_count == 0) return;
  BVHNode& left = nodes_.emplace_back(node.first_primitive_offset, left_count);
  const uint32_t left_index = static_cast<uint32_t>(nodes_.size()) - 1;
  BVHNode& right = nodes_.emplace_back(node.first_primitive_offset + left_count, right_count);
  node.first_primitive_offset = left_index;
  node.primitive_count = 0;
  node.split_axis = best_axis;
  ComputeNodeAABB(left);
  ComputeNodeAABB(right);
  Partition(left);
  Partition(right);
}

bool BVH::TraverseRecursive(const BVHNode& node, const Ray& ray, float t_min, float t_max, Collision& collision) const {
  if (!node.bounding_box.Collide(ray, t_min, t_max)) return false;
  if (node.primitive_count > 0) {
    bool collided = false;
    for (uint32_t i = 0; i < node.primitive_count; ++i) {
      if (std::visit([&](const auto& primitive) { return primitive.Collide(ray, t_min, t_max, collision); },
                     primitives_[node.first_primitive_offset + i])) {
        collided = true;
        t_max = collision.t;
      }
    }
    return collided;
  } else {
    const BVHNode& left = nodes_[node.first_primitive_offset];
    const BVHNode& right = nodes_[node.first_primitive_offset + 1];
    if (ray.InverseDirection()[node.split_axis] < 0.0f) {
      const bool collided_right = TraverseRecursive(right, ray, t_min, t_max, collision);
      const bool collided_left = TraverseRecursive(left, ray, t_min, collided_right ? collision.t : t_max, collision);
      return collided_right || collided_left;
    } else {
      const bool collided_left = TraverseRecursive(left, ray, t_min, t_max, collision);
      const bool collided_right = TraverseRecursive(right, ray, t_min, collided_left ? collision.t : t_max, collision);
      return collided_left || collided_right;
    }
  }
}

} // namespace rt
