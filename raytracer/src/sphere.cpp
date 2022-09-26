#include "sphere.h"

#include <cmath>

namespace rt {
Sphere::Sphere(glm::vec3 center, float radius, const Material* material)
    : center_{center}, radius_{radius}, material_{material} {}

bool Sphere::CollideImpl(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  // Quadratic equation:
  // a * t ^ 2 + b * t + c = 0
  // Let:
  // a = (d.x ^ 2 + d.y ^ 2 + d.z ^ 2), where d = ray direction
  // b = 2 * (p.x * d.x + p.y * d.y + p.z * d.z), where p = point in space, d = ray direction
  // c = (p.x ^ 2 + p.y ^ 2 + p.z ^ 2 - r ^ 2), where p = point in space, r = circle radius
  const glm::vec3 p = ray.Origin() - center_;
  const float a = glm::dot(ray.Direction(), ray.Direction());
  const float b = 2.0f * glm::dot(p, ray.Direction());
  const float c = glm::dot(p, p) - radius_ * radius_;

  // Quadratic equation discriminant:
  // b ^ 2 - 4 * a * c
  const float discriminant = b * b - 4.0f * a * c;
  if (discriminant < 0.0f) return false;

  float root = (-b - std::sqrtf(discriminant)) / (2.0f * a);
  if (root < t_min || t_max < root) {
    root = (-b + std::sqrt(discriminant)) / (2.0f * a);
    if (root < t_min || t_max < root) return false;
  }

  collision.t = root;
  collision.point = ray.At(collision.t);
  glm::vec3 outward_normal = (collision.point - center_) / radius_;
  collision.SetNormal(ray, outward_normal);
  collision.material = material_;

  return true;
}

MovingSphere::MovingSphere(glm::vec3 center0,
                           glm::vec3 center1,
                           float time0,
                           float time1,
                           float radius,
                           const Material* material)
    : center0_{center0}, center1_{center1}, time0_{time0}, time1_{time1}, radius_{radius}, material_{material} {}

bool MovingSphere::CollideImpl(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  // Quadratic equation:
  // a * t ^ 2 + b * t + c = 0
  // Let:
  // a = (d.x ^ 2 + d.y ^ 2 + d.z ^ 2), where d = ray direction
  // b = 2 * (p.x * d.x + p.y * d.y + p.z * d.z), where p = point in space, d = ray direction
  // c = (p.x ^ 2 + p.y ^ 2 + p.z ^ 2 - r ^ 2), where p = point in space, r = circle radius
  const glm::vec3 p = ray.Origin() - CenterAt(ray.Time());
  const float a = glm::dot(ray.Direction(), ray.Direction());
  const float b = 2.0f * glm::dot(p, ray.Direction());
  const float c = glm::dot(p, p) - radius_ * radius_;

  // Quadratic equation discriminant:
  // b ^ 2 - 4 * a * c
  const float discriminant = b * b - 4.0f * a * c;
  if (discriminant < 0.0f) return false;

  float root = (-b - std::sqrtf(discriminant)) / (2.0f * a);
  if (root < t_min || t_max < root) {
    root = (-b + std::sqrt(discriminant)) / (2.0f * a);
    if (root < t_min || t_max < root) return false;
  }

  collision.t = root;
  collision.point = ray.At(collision.t);
  glm::vec3 outward_normal = (collision.point - CenterAt(ray.Time())) / radius_;
  collision.SetNormal(ray, outward_normal);
  collision.material = material_;

  return true;
}
glm::vec3 MovingSphere::CenterAt(float time) const {
  return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
}
}  // namespace rt
