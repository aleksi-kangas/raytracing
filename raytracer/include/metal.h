#pragma once

#include "glm/glm.hpp"

#include "collision.h"
#include "material.h"
#include "ray.h"

namespace rt {
class Metal : public Material {
 public:
  explicit Metal(glm::vec3 albedo, float fuzziness);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const override;

 private:
  glm::vec3 albedo_{0, 0, 0};
  float fuzziness_;
};
}  // namespace rt
