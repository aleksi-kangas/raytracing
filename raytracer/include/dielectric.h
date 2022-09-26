#pragma once

#include "collision.h"
#include "material.h"
#include "ray.h"

namespace rt {
class Dielectric : public Material {
 public:
  explicit Dielectric(float refraction_index);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const override;

 private:
  float refraction_index_;

  static float Reflectance(float cosine, float refraction_index);
};
}  // namespace rt
