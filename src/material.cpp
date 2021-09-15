#include "material.h"

#include <algorithm>
#include <cmath>
#include <utility>

#include "random.h"
#include "utils.h"

Lambertian::Lambertian(const Color &albedo) : albedo_(std::make_shared<SolidColorTexture>(albedo)) {}

Lambertian::Lambertian(std::shared_ptr<Texture> albedo) : albedo_(std::move(albedo)) {}

bool Lambertian::Scatter(const Ray &ray,
                         const Collision &collision,
                         Color &attenuation,
                         Ray &scattered_ray,
                         double &pdf) const {
  Vector3D scatter_direction = Vector3D::RandomInHemisphere(collision.normal);
//  if (scatter_direction.IsNearZero()) {
//    scatter_direction = collision.normal;
//  }
  scattered_ray = Ray(collision.point, scatter_direction, ray.Time());
  attenuation = albedo_->SampleColor(collision.u, collision.v, collision.point);
  pdf = 0.5 / utils::kPI;
  return true;
}

double Lambertian::ScatteringPDF(const Ray &ray, const Collision &collision, const Ray &scattered_ray) const {
  double cosine = Vector3D::DotProduct(collision.normal, scattered_ray.Direction().UnitVector());
  return cosine < 0 ? 0 : cosine / utils::kPI;
}

Metal::Metal(const Color &albedo, double fuzziness) : albedo_(albedo), fuzziness_(std::clamp(fuzziness, 0.0, 1.0)) {}

bool Metal::Scatter(const Ray &ray,
                    const Collision &collision,
                    Color &attenuation,
                    Ray &scattered_ray,
                    double &pdf) const {
  Vector3D reflected = Vector3D::Reflect(ray.Direction().UnitVector(), collision.normal);
  scattered_ray = Ray(collision.point, reflected + fuzziness_ * Vector3D::RandomInUnitSphere(), ray.Time());
  attenuation = albedo_;
  return Vector3D::DotProduct(scattered_ray.Direction(), collision.normal) > 0;
}

Dielectric::Dielectric(double refraction_ratio) : refraction_ratio_(refraction_ratio) {}

bool Dielectric::Scatter(const Ray &ray,
                         const Collision &collision,
                         Color &attenuation,
                         Ray &scattered_ray,
                         double &pdf) const {
  attenuation = Color(1.0, 1.0, 1.0);
  double refraction_ratio = collision.is_front_face ? (1.0 / refraction_ratio_) : refraction_ratio_;
  Vector3D unit_direction = ray.Direction().UnitVector();

  double cos_theta = std::min(Vector3D::DotProduct(-unit_direction, collision.normal), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  bool can_refract = refraction_ratio * sin_theta <= 1.0;
  Vector3D direction;
  if (can_refract && ComputeReflectance(cos_theta, refraction_ratio) <= RandomDouble()) {
    direction = Vector3D::Refract(unit_direction, collision.normal, refraction_ratio);
  } else {
    direction = Vector3D::Reflect(unit_direction, collision.normal);
  }
  scattered_ray = Ray(collision.point, direction, ray.Time());
  return true;
}

double Dielectric::ComputeReflectance(double cosine, double refraction_ratio) {
  // Schlick's approximation https://en.wikipedia.org/wiki/Schlick%27s_approximation.
  double r0 = (1.0 - refraction_ratio) / (1.0 + refraction_ratio);
  r0 = r0 * r0;
  return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5);
}

DiffuseLight::DiffuseLight(const Color &albedo) : emit_(std::make_shared<SolidColorTexture>(albedo)) {}

DiffuseLight::DiffuseLight(std::shared_ptr<Texture> albedo) : emit_(std::move(albedo)) {}

Color DiffuseLight::Emit(double u, double v, const Point3D &point) const {
  return emit_->SampleColor(u, v, point);
}

Isotropic::Isotropic(std::shared_ptr<Texture> albedo) : albedo_(std::move(albedo)) {}

Isotropic::Isotropic(const Color &albedo) : albedo_(std::make_shared<SolidColorTexture>(albedo)) {}

bool Isotropic::Scatter(const Ray &ray,
                        const Collision &collision,
                        Color &attenuation,
                        Ray &scattered_ray,
                        double &pdf) const {
  scattered_ray = Ray(collision.point, Vector3D::RandomInUnitSphere(), ray.Time());
  attenuation = albedo_->SampleColor(collision.u, collision.v, collision.point);
  return true;
}
