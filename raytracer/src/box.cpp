#include "box.h"

#include <utility>

namespace rt {
Box::Box(glm::vec3 min_point, glm::vec3 max_point, material_t material)
    : min_point_{min_point},
      max_point_{max_point},
      material_{std::move(material)},
      sides_{{RectangleXY{glm::vec2{min_point_.x, max_point_.x}, glm::vec2{min_point_.y, max_point.y}, min_point_.z,
                          material_},
              RectangleXY{glm::vec2{min_point_.x, max_point_.x}, glm::vec2{min_point_.y, max_point.y}, max_point_.z,
                          material_},
              RectangleXZ{glm::vec2{min_point_.x, max_point_.x}, glm::vec2{min_point_.z, max_point_.z}, min_point_.y,
                          material_},
              RectangleXZ{glm::vec2{min_point_.x, max_point_.x}, glm::vec2{min_point_.z, max_point_.z}, max_point_.y,
                          material_},
              RectangleYZ{glm::vec2{min_point_.y, max_point_.y}, glm::vec2{min_point_.z, max_point_.z}, min_point_.x,
                          material_},
              RectangleYZ{glm::vec2{min_point_.y, max_point_.y}, glm::vec2{min_point_.z, max_point_.z}, max_point_.x,
                          material_}}} {}

bool Box::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  bool collided;
  float closest = t_max;
  for (const auto& collidable : sides_) {
    if (std::visit([&](const auto& rectangle) { return rectangle.Collide(ray, t_min, closest, collision); },
                   collidable)) {
      collided = true;
      closest = collision.t;
    }
  }
  return collided;
}

bool Box::BoundingBox(float time0, float time1, AABB& bounding_box) const {
  bounding_box = AABB{min_point_, max_point_};
  return true;
}

glm::vec3 Box::Centroid() const {
  return min_point_ + (max_point_ - min_point_) * 0.5f;
}
}  // namespace rt
