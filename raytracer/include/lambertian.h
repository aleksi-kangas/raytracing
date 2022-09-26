#pragma once

#include "glm/glm.hpp"

#include "collision.h"
#include "material.h"
#include "ray.h"

namespace rt {
class Lambertian : public Material {
 public:
  explicit Lambertian(glm::vec3 albedo);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const override;

 private:
  glm::vec3 albedo_{0, 0, 0};
};
}  // namespace rt
