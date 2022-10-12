#include "flip.h"

#include <utility>

namespace rt {

Flip::Flip(primitive_t primitive) : primitive_(std::move(primitive)) {}

bool Flip::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  if (!std::visit([&](const auto& primitive) { return primitive.Collide(ray, t_min, t_max, collision); }, primitive_)) {
    return false;
  }
  collision.outside = !collision.outside;
  return true;
}

bool Flip::BoundingBox(float time0, float time1, AABB& bounding_box) const {
  return std::visit([&](const auto& primitive) { return primitive.BoundingBox(time0, time1, bounding_box); },
                    primitive_);
}

glm::vec3 Flip::Centroid() const {
  return std::visit([](const auto& primitive) { return primitive.Centroid(); }, primitive_);
}

float Flip::PDFValue(const glm::vec3& origin, const glm::vec3& direction) const {
  // TODO
  return 0.0f;
}

glm::vec3 Flip::RandomTowards(const glm::vec3& origin) const {
  // TODO
  return {0.0f, 0.0f, 0.0f};
}

}  // namespace rt
