#include "constant_medium.h"

#include <limits>
#include <utility>

#include "random.h"

namespace rt {
ConstantMedium::ConstantMedium(primitive_t boundary, float density, texture_t texture)
    : boundary_{std::move(boundary)},
      negative_inverse_density_{-1.0f / density},
      phase_function_{Isotropic{std::move(texture)}} {}

bool ConstantMedium::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  Collision collision_1, collision_2;
  if (!std::visit([&](const auto& primitive) {
    return primitive.Collide(ray,
                             std::numeric_limits<float>::lowest(),
                             std::numeric_limits<float>::max(),
                             collision_1);
  }, boundary_))
    return false;
  if (!std::visit([&](const auto& primitive) {
    return primitive.Collide(ray,
                             collision_1.t + 0.0001f,
                             std::numeric_limits<float>::max(),
                             collision_2);
  }, boundary_))
    return false;
  collision_1.t = glm::max(collision_1.t, t_min);
  collision_2.t = glm::min(collision_2.t, t_max);
  if (collision_1.t >= collision_2.t) return false;
  collision_1.t = glm::max(collision_1.t, 0.0f);

  const float ray_length = glm::length(ray.Direction());
  const float distance_inside_boundary = (collision_2.t - collision_1.t) * ray_length;
  const float hit_distance = negative_inverse_density_ * glm::log(random::Float());
  if (hit_distance > distance_inside_boundary) return false;
  collision.t = collision_1.t + hit_distance / ray_length;
  collision.point = ray.At(collision.t);
  collision.material = &phase_function_;
  return true;
}

bool ConstantMedium::BoundingBox(float time0, float time1, AABB& bounding_box) const {
  return std::visit([&](const auto& primitive) { return primitive.BoundingBox(time0, time1, bounding_box); },
                    boundary_);
}

glm::vec3 ConstantMedium::Centroid() const {
  return std::visit([](const auto& primitive) { return primitive.Centroid(); }, boundary_);
}

float ConstantMedium::PDFValue(const glm::vec3& origin, const glm::vec3& direction) const {
  // TODO
  return 0.0f;
}

glm::vec3 ConstantMedium::RandomTowards(const glm::vec3& origin) const {
  // TODO
  return {0.0f, 0.0f, 0.0f};
}

}  // namespace rt
