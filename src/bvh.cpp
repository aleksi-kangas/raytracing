#include "bvh.h"

#include <exception>
#include <functional>
#include <string>
#include <utility>

#include "random.h"

namespace {
class BVHException : public std::exception {
 public:
  explicit BVHException(std::string message = "BVH could not be constructed.") : message_(std::move(message)) {}

  [[nodiscard]] const char *what() const noexcept override {
    return message_.c_str();
  }

 private:
  std::string message_;
};
}  // namespace anonymous

BoundingVolumeHierarchyNode::BoundingVolumeHierarchyNode(const Collidables &collidables, double time0, double time1)
    : BoundingVolumeHierarchyNode(collidables.CollidableObjects(),
                                  0,
                                  static_cast<int>(collidables.CollidableObjects().size()),
                                  time0,
                                  time1) {}

BoundingVolumeHierarchyNode::BoundingVolumeHierarchyNode(const std::vector<std::shared_ptr<Collidable>> &collidable_objects,
                                                         int start,
                                                         int end,
                                                         double time0,
                                                         double time1) {
  if (collidable_objects.empty() || collidable_objects.size() == 1) {
    throw BVHException("BVH requires 2 or more collidable objects.");
  }

  std::vector<std::shared_ptr<Collidable>> objects = collidable_objects;  // Make a copy.
  int axis = RandomInt(0, 2);
  auto comparator = (axis == 0) ? BoxCompareX : (axis == 1) ? BoxCompareY : BoxCompareZ;

  // Optimizations by Johann Meyer: https://github.com/RayTracing/raytracing.github.io/issues/890
  int span = end - start;
  if (span == 3) {
    left_ = std::make_shared<BoundingVolumeHierarchyNode>(objects, start, start + 2, time0, time1);
    right_ = std::move(objects[start + 2]);
  } else if (span == 2) {
    left_ = std::move(objects[start]);
    right_ = std::move(objects[start + 1]);
  } else {
    std::sort(objects.begin() + start, objects.begin() + end, comparator);
    size_t mid = start + span / 2;
    left_ = std::make_shared<BoundingVolumeHierarchyNode>(objects, start, mid, time0, time1);
    right_ = std::make_shared<BoundingVolumeHierarchyNode>(objects, mid, end, time0, time1);
  }

  AxisAlignedBoundingBox left_box, right_box;
  if (!left_->BoundingBox(time0, time1, left_box) || !right_->BoundingBox(time0, time1, right_box)) {
    throw BVHException("No bounding box found.");
  }
  bounding_box_ = AxisAlignedBoundingBox::SurroundingBox(left_box, right_box);
}

bool BoundingVolumeHierarchyNode::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  if (!bounding_box_.Collide(ray, t_min, t_max)) {
    return false;
  }
  bool collided_left = left_->Collide(ray, t_min, t_max, collision);
  bool collided_right = right_->Collide(ray, t_min, collided_left ? collision.t : t_max, collision);
  return collided_left || collided_right;
}

bool BoundingVolumeHierarchyNode::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  bounding_box = bounding_box_;
  return true;
}

bool BoundingVolumeHierarchyNode::BoxCompare(const std::shared_ptr<Collidable> &a,
                                             const std::shared_ptr<Collidable> &b,
                                             int axis) {
  AxisAlignedBoundingBox box_a, box_b;
  if (!a->BoundingBox(0.0, 0.0, box_a) || !b->BoundingBox(0.0, 0.0, box_b)) {
    throw BVHException("No bounding box found.");
  }
  return box_a.MinPoint()[axis] < box_b.MinPoint()[axis];
}

bool BoundingVolumeHierarchyNode::BoxCompareX(const std::shared_ptr<Collidable> &a,
                                              const std::shared_ptr<Collidable> &b) {
  return BoxCompare(a, b, 0);
}

bool BoundingVolumeHierarchyNode::BoxCompareY(const std::shared_ptr<Collidable> &a,
                                              const std::shared_ptr<Collidable> &b) {
  return BoxCompare(a, b, 1);
}

bool BoundingVolumeHierarchyNode::BoxCompareZ(const std::shared_ptr<Collidable> &a,
                                              const std::shared_ptr<Collidable> &b) {
  return BoxCompare(a, b, 2);
}
