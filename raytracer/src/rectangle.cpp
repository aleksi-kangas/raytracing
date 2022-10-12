#include "rectangle.h"

#include <utility>

#include "random.h"

namespace rt {
RectangleXY::RectangleXY(glm::vec2 x, glm::vec2 y, float z, material_t material)
    : x_{x}, y_{y}, z_{z}, material_{std::move(material)} {}

bool RectangleXY::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  const float t = (z_ - ray.Origin().z) / ray.Direction().z;
  if (t < t_min || t_max < t) return false;
  const auto plane_point = ray.Origin() + t * ray.Direction();
  if (plane_point.x < x_[0] || x_[1] < plane_point.x || plane_point.y < y_[0] || y_[1] < plane_point.y) return false;
  collision.point = ray.At(t);
  collision.u = (plane_point.x - x_[0]) / (x_[1], x_[0]);
  collision.v = (plane_point.y - y_[0]) / (y_[1], y_[0]);
  collision.t = t;
  const glm::vec3 outward_normal{0, 0, 1};
  collision.SetNormal(ray, outward_normal);
  collision.material = &material_;
  return true;
}

bool RectangleXY::BoundingBox(float time0, float time1, AABB& bounding_box) const {
  bounding_box = AABB{{x_[0], y_[0], z_ - 0.0001f}, {x_[1], y_[1], z_ + 0.0001f}};
  return true;
}

glm::vec3 RectangleXY::Centroid() const {
  return (glm::vec3{x_[0], y_[0], z_} + glm::vec3{x_[1], y_[1], z_}) / 2.0f;
}

float RectangleXY::PDFValue(const glm::vec3& origin, const glm::vec3& direction) const {
  // TODO
  return 0.0f;
}

glm::vec3 RectangleXY::RandomTowards(const glm::vec3& origin) const {
  // TODO
  return {0.0f, 0.0f, 0.0f};
}

RectangleXZ::RectangleXZ(glm::vec2 x, glm::vec2 z, float y, material_t material)
    : x_{x}, z_{z}, y_{y}, material_{std::move(material)} {}

bool RectangleXZ::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  const float t = (y_ - ray.Origin().y) / ray.Direction().y;
  if (t < t_min || t_max < t) return false;
  const auto plane_point = ray.Origin() + t * ray.Direction();
  if (plane_point.x < x_[0] || x_[1] < plane_point.x || plane_point.z < z_[0] || z_[1] < plane_point.z) return false;
  collision.point = ray.At(t);
  collision.u = (plane_point.x - x_[0]) / (x_[1] - x_[0]);
  collision.v = (plane_point.z - z_[0]) / (z_[1] - z_[0]);
  collision.t = t;
  const glm::vec3 outward_normal{0, 1, 0};
  collision.SetNormal(ray, outward_normal);
  collision.material = &material_;
  return true;
}

bool RectangleXZ::BoundingBox(float time0, float time1, AABB& bounding_box) const {
  bounding_box = AABB{{x_[0], y_ - 0.0001f, z_[0]}, {x_[1], y_ + 0.0001f, z_[1]}};
  return true;
}

glm::vec3 RectangleXZ::Centroid() const {
  return (glm::vec3{x_[0], z_[0], y_} + glm::vec3{x_[1], z_[1], y_}) / 2.0f;
}

float RectangleXZ::PDFValue(const glm::vec3& origin, const glm::vec3& direction) const {
  Collision collision;
  if (!Collide(Ray{origin, direction}, 0.001f, std::numeric_limits<float>::max(), collision)) return 0.0f;
  const float area = (x_[1] - x_[0]) * (z_[1] - z_[0]);
  const float distance_squared = collision.t * collision.t * glm::dot(direction, direction);
  const float cosine = glm::abs(glm::dot(direction, collision.normal) / glm::length(direction));
  return distance_squared / (cosine * area);
}

glm::vec3 RectangleXZ::RandomTowards(const glm::vec3& origin) const {
  const glm::vec3 random_point{random::Float(x_[0], x_[1]), y_, random::Float(z_[0], z_[1])};
  return random_point - origin;
}

RectangleYZ::RectangleYZ(glm::vec2 y, glm::vec2 z, float x, material_t material)
    : y_{y}, z_{z}, x_{x}, material_{std::move(material)} {}

bool RectangleYZ::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  const float t = (x_ - ray.Origin().x) / ray.Direction().x;
  if (t < t_min || t_max < t) return false;
  const auto plane_point = ray.Origin() + t * ray.Direction();
  if (plane_point.y < y_[0] || y_[1] < plane_point.y || plane_point.z < z_[0] || z_[1] < plane_point.z) return false;
  collision.point = ray.At(t);
  collision.u = (plane_point.y - y_[0]) / (y_[1] - y_[0]);
  collision.v = (plane_point.z - z_[0]) / (z_[1] - z_[0]);
  collision.t = t;
  const glm::vec3 outward_normal{1, 0, 0};
  collision.SetNormal(ray, outward_normal);
  collision.material = &material_;
  return true;
}

bool RectangleYZ::BoundingBox(float time0, float time1, AABB& bounding_box) const {
  bounding_box = AABB{{x_ - 0.0001f, y_[0], z_[0]}, {x_ + 0.0001f, y_[1], z_[1]}};
  return true;
}

glm::vec3 RectangleYZ::Centroid() const {
  return (glm::vec3{y_[0], z_[0], x_} + glm::vec3{y_[1], z_[1], x_}) / 2.0f;
}

float RectangleYZ::PDFValue(const glm::vec3& origin, const glm::vec3& direction) const {
  // TODO
  return 0.0f;
}

glm::vec3 RectangleYZ::RandomTowards(const glm::vec3& origin) const {
  // TODO
  return {0.0f, 0.0f, 0.0f};
}

}  // namespace rt
