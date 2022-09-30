#pragma once

#include "glm/glm.hpp"

#include "collision.h"
#include "material.h"
#include "ray.h"
#include "texture.h"

namespace rt {
class Lambertian : public Material {
 public:
  explicit Lambertian(const Texture* albedo);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const override;

 private:
  const Texture* albedo_;
};
}  // namespace rt
