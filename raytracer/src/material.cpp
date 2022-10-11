#include "material.h"

#include <numbers>
#include <utility>
#include <variant>

#include "collision.h"
#include "onb.h"
#include "random.h"
#include "utils.h"

namespace rt {
Dielectric::Dielectric(float refraction_index) : refraction_index_{refraction_index} {}

bool Dielectric::Scatter(const Ray& ray,
                         const Collision& collision,
                         glm::vec3& attenuation,
                         Ray& scattered,
                         float& pdf) const {
  attenuation = {1, 1, 1};
  const float refraction_ratio = collision.outside ? (1.0f / refraction_index_) : refraction_index_;

  const glm::vec3 unit_direction = glm::normalize(ray.Direction());
  const float cos_theta = std::fmin(glm::dot(-unit_direction, collision.normal), 1.0f);
  const float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

  const bool can_refract =
      refraction_ratio * sin_theta <= 1.0f && Reflectance(cos_theta, refraction_ratio) <= random::Float();
  const glm::vec3 direction = can_refract ?
                              glm::refract(unit_direction, collision.normal, refraction_ratio) :
                              glm::reflect(unit_direction, collision.normal);
  scattered = Ray{collision.point, direction, ray.Time()};
  return true;
}

float Dielectric::ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const {
  return 0.0f;
}

glm::vec3 Dielectric::Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const {
  return {0, 0, 0};
}

float Dielectric::Reflectance(float cosine, float refraction_index) {
  // Schlick's approximation.
  float r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
  r0 *= r0;
  return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
}

DiffuseLight::DiffuseLight(glm::vec3 emit) : emit_{SolidColorTexture{emit}} {}

DiffuseLight::DiffuseLight(texture_t emit) : emit_{std::move(emit)} {}

bool DiffuseLight::Scatter(const Ray& ray,
                           const Collision& collision,
                           glm::vec3& attenuation,
                           Ray& scattered,
                           float& pdf) const {
  return false;
}

float DiffuseLight::ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const {
  return 0.0f;
}

glm::vec3 DiffuseLight::Emit(const Ray& ray,
                             const Collision& collision,
                             float u,
                             float v,
                             const glm::vec3& point) const {
  if (collision.outside) {
    return std::visit([&](const auto& texture) { return texture.Sample(u, v, point); }, emit_);
  } else {
    return {0, 0, 0};
  }
}

Isotropic::Isotropic(glm::vec3 albedo) : albedo_{SolidColorTexture{albedo}} {}

Isotropic::Isotropic(texture_t albedo) : albedo_{std::move(albedo)} {}

bool Isotropic::Scatter(const Ray& ray,
                        const Collision& collision,
                        glm::vec3& attenuation,
                        Ray& scattered,
                        float& pdf) const {
  attenuation = std::visit([&](const auto& texture) {
    return texture.Sample(collision.u, collision.v, collision.point);
  }, albedo_);
  scattered = Ray{collision.point, random::InUnitSphere(), ray.Time()};
  return true;
}

float Isotropic::ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const {
  return 0.0f;
}

glm::vec3 Isotropic::Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const {
  return {0, 0, 0};
}

Lambertian::Lambertian(texture_t albedo) : albedo_{std::move(albedo)} {}

bool Lambertian::Scatter(const Ray& ray,
                         const Collision& collision,
                         glm::vec3& attenuation,
                         Ray& scattered,
                         float& pdf) const {
  const ONB onb{collision.normal};
  const glm::vec3 scatter_direction = onb.Local(random::CosineDirection());
  scattered = Ray{collision.point, glm::normalize(scatter_direction), ray.Time()};
  attenuation = std::visit([&](const auto& texture) {
    return texture.Sample(collision.u, collision.v, collision.point);
  }, albedo_);
  pdf = glm::dot(onb.w, scattered.Direction()) / std::numbers::pi_v<float>;
  return true;
}

float Lambertian::ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const {
  const float cosine = glm::dot(collision.normal, glm::normalize(scattered.Direction()));
  return cosine < 0.0f ? 0.0f : cosine / std::numbers::pi_v<float>;
}

glm::vec3 Lambertian::Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const {
  return {0, 0, 0};
}

Metal::Metal(glm::vec3 albedo, float fuzziness)
    : albedo_{albedo}, fuzziness_{fuzziness < 1.0f ? fuzziness : 1.0f} {}

bool Metal::Scatter(const Ray& ray,
                    const Collision& collision,
                    glm::vec3& attenuation,
                    Ray& scattered,
                    float& pdf) const {
  const glm::vec3 reflected = glm::reflect(glm::normalize(ray.Direction()), collision.normal);
  scattered = Ray{collision.point, reflected + fuzziness_ * random::InUnitSphere(), ray.Time()};
  attenuation = albedo_;
  return glm::dot(scattered.Direction(), collision.normal) > 0;
}

float Metal::ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const {
  return 0.0f;
}

glm::vec3 Metal::Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const {
  return {0, 0, 0};
}

}  // namespace rt
