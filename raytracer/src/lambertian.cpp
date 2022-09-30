#include "lambertian.h"

#include "random.h"
#include "utils.h"

namespace rt {
Lambertian::Lambertian(const Texture* albedo) : albedo_{albedo} {}

bool Lambertian::Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const {
  glm::vec3 scatter_direction = collision.normal + random::UnitVec3();
  if (utils::IsNearZero(scatter_direction)) {
    scatter_direction = collision.normal;
  }
  scattered = Ray{collision.point, scatter_direction, ray.Time()};
  attenuation = albedo_->Sample(collision.u, collision.v, collision.point);
  return true;
}

}  // namespace rt
