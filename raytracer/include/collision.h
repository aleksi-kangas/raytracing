#pragma once

#include "glm/glm.hpp"

#include "material.h"
#include "ray.h"

namespace rt {
struct Collision {
  glm::vec3 point{0, 0, 0};
  glm::vec3 normal{0, 0, 0};
  const Material* material = nullptr;
  float t = 0.0f;
  float u = 0.0f;
  float v = 0.0f;
  bool outside = false;

  inline void SetNormal(const Ray& ray, const glm::vec3& outward_normal) {
    outside = glm::dot(ray.Direction(), outward_normal) < 0.0f;
    normal = outside ? outward_normal : -outward_normal;
  }
};
}  // namespace rt
