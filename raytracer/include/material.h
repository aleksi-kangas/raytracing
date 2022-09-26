#pragma once

#include "glm/glm.hpp"

#include "ray.h"

namespace rt {
struct Collision;

class Material {
 public:
  virtual ~Material() = default;
  virtual bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const = 0;
};
}  // namespace rt
