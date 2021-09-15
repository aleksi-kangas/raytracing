#include "material.h"

#include <algorithm>

Lambertian::Lambertian(const Color &albedo) : albedo_(albedo) {}

bool Lambertian::Scatter(const Ray &ray, const Collision &collision, Color &attenuation, Ray &scattered_ray) const {
  Vector3D scatter_direction = collision.normal + Vector3D::RandomUnitVector();
  if (scatter_direction.IsNearZero()) {
    scatter_direction = collision.normal;
  }
  scattered_ray = Ray(collision.point, scatter_direction);
  attenuation = albedo_;
  return true;
}

Metal::Metal(const Color &albedo, double fuzziness) : albedo_(albedo), fuzziness_(std::clamp(fuzziness, 0.0, 1.0)) {}

bool Metal::Scatter(const Ray &ray, const Collision &collision, Color &attenuation, Ray &scattered_ray) const {
  Vector3D reflected = Vector3D::Reflect(ray.Direction().UnitVector(), collision.normal);
  scattered_ray = Ray(collision.point, reflected + fuzziness_ * Vector3D::RandomInUnitSphere());
  attenuation = albedo_;
  return Vector3D::DotProduct(scattered_ray.Direction(), collision.normal) > 0;
}
