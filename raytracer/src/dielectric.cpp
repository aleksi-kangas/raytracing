#include "dielectric.h"

#include <cmath>

#include "random.h"

namespace rt {
Dielectric::Dielectric(float refraction_index) : refraction_index_{refraction_index} {}

bool Dielectric::Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const {
  attenuation = {1, 1, 1};
  const float refraction_ratio = collision.outside ? (1.0f / refraction_index_) : refraction_index_;

  const glm::vec3 unit_direction = glm::normalize(ray.Direction());
  const float cos_theta = std::fmin(glm::dot(-unit_direction, collision.normal), 1.0f);
  const float sin_theta = std::sqrtf(1.0f - cos_theta * cos_theta);

  const bool can_refract =
      refraction_ratio * sin_theta <= 1.0f && Reflectance(cos_theta, refraction_ratio) <= random::Float();
  const glm::vec3 direction = can_refract ?
                              glm::refract(unit_direction, collision.normal, refraction_ratio) :
                              glm::reflect(unit_direction, collision.normal);
  scattered = Ray{collision.point, direction, ray.Time()};
  return true;
}

float Dielectric::Reflectance(float cosine, float refraction_index) {
  // Schlick's approximation.
  float r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
  r0 *= r0;
  return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
}

}  // namespace rt
