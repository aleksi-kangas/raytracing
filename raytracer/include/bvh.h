#pragma once

#include <immintrin.h>

#include <algorithm>
#include <cstdint>
#include <limits>
#include <stack>
#include <thread>
#include <vector>

#include "aabb.h"
#include "collidable.h"
#include "collision.h"
#include "random.h"
#include "ray.h"
#include "rectangle.h"
#include "sphere.h"

namespace rt {
/**
 * Defines the strategy for splits when building a BVH-tree.
 */
enum BVHSplitStrategy { LargestExtent, SurfaceAreaHeuristic, SplitStrategyCount };

/**
 * Defines the strategy when traversing a BVH-tree.
 */
enum BVHTraversalStrategy { Recursive, Iterative, TraversalStrategyCount };

/**
 * Boundary Volume Hierarchy implementation from a great blog series by Jacco Bikker. (https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/)
 * This implementation supports two split strategies -- largest extent & surface-area-heuristic (SAH).
 * Traversal supports both recursive and iterative forms.
 * @tparam T Primitive type implementing Collidable interface.
 */
class BVH : public Collidable<BVH> {
 public:
  BVH(BVHSplitStrategy split_strategy,
      BVHTraversalStrategy traversal_strategy,
      const collidable_container_t& primitives,
      float time0,
      float time1)
      : split_strategy_{split_strategy},
        traversal_strategy_{traversal_strategy},
        primitives_{primitives},
        time0_{time0},
        time1_{time1} {
    Build();
  }

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
    switch (traversal_strategy_) {
      case BVHTraversalStrategy::Recursive:
        return TraverseRecursive(nodes_[0], ray, t_min, t_max, collision);
      case BVHTraversalStrategy::Iterative:
        return TraverseIterative(ray, t_min, t_max, collision);
      default:
        return false;
    }
  }

 private:
  BVHSplitStrategy split_strategy_;
  BVHTraversalStrategy traversal_strategy_;
  const collidable_container_t& primitives_;

  float time0_ = 0.0f, time1_ = 0.0f;
  struct BVHNode {
    AABB bounding_box;
    uint32_t left_first_primitive = 0, primitive_count = 0;
    uint32_t split_axis = 0;
    [[nodiscard]] bool IsLeaf() const { return primitive_count > 0; }
  };
  std::vector<BVHNode> nodes_;
  uint32_t nodes_used = 0;
  std::vector<uint32_t> primitive_indices_;

  bool TraverseIterative(const Ray& ray, float t_min, float t_max, Collision& collision) const {
    bool collided = false;
    float closest = t_max;
    std::stack<const BVHNode*> stack;
    stack.push(&nodes_[0]);
    while (!stack.empty()) {
      const BVHNode* node = stack.top();
      stack.pop();
      if (!node->bounding_box.Collide(ray, t_min, closest)) continue;
      if (node->primitive_count) {
        for (uint32_t i = 0; i < node->primitive_count; ++i) {
          const auto& primitive = primitives_[primitive_indices_[node->left_first_primitive + i]];
          if (std::visit([&](const auto& collidable) { return collidable.Collide(ray, t_min, closest, collision); },
                         primitive)) {
            collided = true;
            closest = collision.t;
          }
        }
      } else {
        const BVHNode* left = &nodes_[node->left_first_primitive];
        const BVHNode* right = &nodes_[node->left_first_primitive + 1];
        if (ray.InverseDirection()[node->split_axis] < 0) {  // Should visit right child first.
          stack.push(left);
          stack.push(right);
        } else {  // Should visit left child first.
          stack.push(right);
          stack.push(left);
        }
      }
    }
    return collided;
  }

  bool TraverseRecursive(const BVHNode& node, const Ray& ray, float t_min, float t_max, Collision& collision) const {
    if (!node.bounding_box.Collide(ray, t_min, t_max)) return false;
    if (node.IsLeaf()) {
      bool collided = false;
      float closest = t_max;
      for (uint32_t i = 0; i < node.primitive_count; ++i) {
        const auto& primitive = primitives_[primitive_indices_[node.left_first_primitive + i]];
        if (std::visit([&](const auto& collidable) { return collidable.Collide(ray, t_min, closest, collision); },
                       primitive)) {
          collided = true;
          closest = collision.t;
        }
      }
      return collided;
    } else {
      const BVHNode& left = nodes_[node.left_first_primitive];
      const BVHNode& right = nodes_[node.left_first_primitive + 1];
      if (ray.InverseDirection()[node.split_axis] < 0) {  // Should visit right child first.
        const bool collided_right = TraverseRecursive(right, ray, t_min, t_max, collision);
        const bool collided_left = TraverseRecursive(left, ray, t_min, collided_right ? collision.t : t_max, collision);
        return collided_left || collided_right;
      } else {  // Should visit left child first.
        const bool collided_left = TraverseRecursive(left, ray, t_min, t_max, collision);
        const bool
            collided_right = TraverseRecursive(right, ray, t_min, collided_left ? collision.t : t_max, collision);
        return collided_left || collided_right;
      }
    }
  }

  void ComputeAABB(BVHNode& node) {
    glm::vec3 b_box_min = glm::vec3{std::numeric_limits<float>::max()};
    glm::vec3 b_box_max = glm::vec3{std::numeric_limits<float>::lowest()};
    for (uint32_t i = 0; i < node.primitive_count; ++i) {
      const auto& primitive = primitives_[primitive_indices_[node.left_first_primitive + i]];
      AABB primitive_bounding_box;
      std::visit([&](const auto& collidable) { collidable.BoundingBox(time0_, time1_, primitive_bounding_box); }, primitive);
      b_box_min = glm::min(b_box_min, primitive_bounding_box.MinPoint());
      b_box_max = glm::max(b_box_max, primitive_bounding_box.MaxPoint());
    }
    node.bounding_box = AABB{b_box_min, b_box_max};
  };

  float EvaluateSAH(BVHNode& node, uint32_t axis, float position) {
    AABB left_box, right_box;
    uint32_t left_count = 0, right_count = 0;
    for (uint32_t i = 0; i < node.primitive_count; ++i) {
      const auto& primitive = primitives_[primitive_indices_[node.left_first_primitive + i]];
      AABB primitive_bounding_box;
      std::visit([&](const auto& object) { object.BoundingBox(time0_, time1_, primitive_bounding_box); }, primitive);
      const glm::vec3 centroid = std::visit([](const auto& collidable) { return collidable.Centroid(); }, primitive);
      if (centroid[axis] < position) {
        ++left_count;
        left_box = AABB{glm::min(left_box.MinPoint(), primitive_bounding_box.MinPoint()),
                        glm::max(left_box.MaxPoint(), primitive_bounding_box.MaxPoint())};
      } else {
        ++right_count;
        right_box = AABB{glm::min(right_box.MinPoint(), primitive_bounding_box.MinPoint()),
                         glm::max(right_box.MaxPoint(), primitive_bounding_box.MaxPoint())};
      }
    }
    const float cost = left_count * left_box.Area() + right_count * right_box.Area();
    return cost > 0.0f ? cost : std::numeric_limits<float>::max();
  }

  void Partition(BVHNode& node) {
    // Determine split axis and position.
    int32_t split_axis = 0;
    float split_position = 0.0f;
    switch (split_strategy_) {
      case BVHSplitStrategy::LargestExtent: {
        if (node.primitive_count <= 2) return;
        // Split along the longest axis of the bounding box.
        const glm::vec3 extent = node.bounding_box.MaxPoint() - node.bounding_box.MinPoint();
        if (extent.y > extent.x) split_axis = 1;
        if (extent.z > extent[split_axis]) split_axis = 2;
        split_position = node.bounding_box.MinPoint()[split_axis] + extent[split_axis] * 0.5f;
      }
        break;
      case BVHSplitStrategy::SurfaceAreaHeuristic: {
        int32_t best_axis = -1;
        float best_position = 0.0f, best_cost = std::numeric_limits<float>::max();
        for (int32_t axis = 0; axis < 3; ++axis) {
          for (uint32_t i = 0; i < node.primitive_count; ++i) {
            const auto& primitive = primitives_[primitive_indices_[node.left_first_primitive + i]];
            const glm::vec3
                centroid = std::visit([](const auto& collidable) { return collidable.Centroid(); }, primitive);
            const float candidate_position = centroid[axis];
            const float cost = EvaluateSAH(node, axis, candidate_position);
            if (cost < best_cost) {
              best_cost = cost;
              best_position = candidate_position;
              best_axis = axis;
            }
          }
        }
        split_axis = best_axis;
        split_position = best_position;
        const float parent_area = node.bounding_box.Area();
        const float parent_cost = node.primitive_count * parent_area;
        if (best_cost >= parent_cost) return;
      }
        break;
    }
    // Perform the partition based on the chosen axis and point.
    int32_t i = node.left_first_primitive;
    int32_t j = i + node.primitive_count - 1;
    while (i <= j) {
      const auto& primitive = primitives_[primitive_indices_[i]];
      const glm::vec3 centroid = std::visit([](const auto& collidable) { return collidable.Centroid(); }, primitive);
      if (centroid[split_axis] < split_position) {
        ++i;
      } else {
        std::swap(primitive_indices_[i], primitive_indices_[j--]);
      }
    }
    const uint32_t left_count = i - node.left_first_primitive;
    if (left_count == 0 || left_count == node.primitive_count) return;
    const uint32_t left_child_index = nodes_used++;
    const uint32_t right_child_index = nodes_used++;
    nodes_[left_child_index].left_first_primitive = node.left_first_primitive;
    nodes_[left_child_index].primitive_count = left_count;
    nodes_[right_child_index].left_first_primitive = i;
    nodes_[right_child_index].primitive_count = node.primitive_count - left_count;
    node.left_first_primitive = left_child_index;
    node.primitive_count = 0;
    ComputeAABB(nodes_[left_child_index]);
    ComputeAABB(nodes_[right_child_index]);
    Partition(nodes_[left_child_index]);
    Partition(nodes_[right_child_index]);
  }

  void Build() {
    const auto n = static_cast<uint32_t>(primitives_.size());
    nodes_.resize(2 * n);
    primitive_indices_.resize(n);
    for (uint32_t i = 0; i < n; ++i) {
      primitive_indices_[i] = i;
    }
    BVHNode& root = nodes_[nodes_used++];
    root.left_first_primitive = 0, root.primitive_count = n;
    ComputeAABB(root);
    Partition(root);
  }
};
}  // namespace rt
