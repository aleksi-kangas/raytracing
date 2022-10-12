#include "sphere.h"

#include <cmath>
#include <numbers>

#include "utils.h"

namespace rt {
Sphere::Sphere(glm::vec3 center, float radius, material_t material)
    : center_{center}, radius_{radius}, material_{material} {}

bool Sphere::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
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

  float root = (-b - sqrtf(discriminant)) / (2.0f * a);
  if (root < t_min || t_max < root) {
    root = (-b + sqrt(discriminant)) / (2.0f * a);
    if (root < t_min || t_max < root) return false;
  }

  collision.t = root;
  collision.point = ray.At(collision.t);
  glm::vec3 outward_normal = (collision.point - center_) / radius_;
  collision.SetNormal(ray, outward_normal);
  ComputeUV(outward_normal, collision.u, collision.v);
  collision.material = &material_;

  return true;
}

bool Sphere::BoundingBox(float time0, float time1, AABB& bounding_box) const {
  bounding_box = AABB{center_ - glm::vec3{radius_, radius_, radius_},
                      center_ + glm::vec3{radius_, radius_, radius_}};
  return true;
}

glm::vec3 Sphere::Centroid() const {
  return center_;
}

float Sphere::PDFValue(const glm::vec3& origin, const glm::vec3& direction) const {
  // TODO
  return 0.0f;
}

glm::vec3 Sphere::RandomTowards(const glm::vec3& origin) const {
  // TODO
  return {0.0f, 0.0f, 0.0f};
}

void Sphere::ComputeUV(const glm::vec3& point, float& u, float& v) {
  const float theta = acos(-point.y);
  const float phi = atan2f(-point.z, point.x) + static_cast<float>(std::numbers::pi);
  u = phi / (2.0f * static_cast<float>(std::numbers::pi));
  v = theta / static_cast<float>(std::numbers::pi);
}

MovingSphere::MovingSphere(glm::vec3 center0,
                           glm::vec3 center1,
                           float time0,
                           float time1,
                           float radius,
                           material_t material)
    : center0_{center0}, center1_{center1}, time0_{time0}, time1_{time1}, radius_{radius}, material_{material} {}

bool MovingSphere::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
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

  float root = (-b - sqrtf(discriminant)) / (2.0f * a);
  if (root < t_min || t_max < root) {
    root = (-b + sqrt(discriminant)) / (2.0f * a);
    if (root < t_min || t_max < root) return false;
  }

  collision.t = root;
  collision.point = ray.At(collision.t);
  glm::vec3 outward_normal = (collision.point - CenterAt(ray.Time())) / radius_;
  collision.SetNormal(ray, outward_normal);
  collision.material = &material_;

  return true;
}

bool MovingSphere::BoundingBox(float time0, float time1, AABB& bounding_box) const {
  AABB box0{center0_ - glm::vec3{radius_, radius_, radius_},
            center0_ + glm::vec3{radius_, radius_, radius_}};
  AABB box1{center1_ - glm::vec3{radius_, radius_, radius_},
            center1_ + glm::vec3{radius_, radius_, radius_}};
  bounding_box = AABB::SurroundingBox(box0, box1);
  return true;
}

glm::vec3 MovingSphere::Centroid() const {
  return center0_ + (center1_ - center0_) / 2.0f;
}

float MovingSphere::PDFValue(const glm::vec3& origin, const glm::vec3& direction) const {
  // TODO
  return 0.0f;
}

glm::vec3 MovingSphere::RandomTowards(const glm::vec3& origin) const {
  // TODO
  return {0.0f, 0.0f, 0.0f};
}

glm::vec3 MovingSphere::CenterAt(float time) const {
  return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
}

}  // namespace rt
