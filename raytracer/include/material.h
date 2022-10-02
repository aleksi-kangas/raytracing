#pragma once

#include <variant>
#include <vector>

#include "glm/glm.hpp"

#include "crtp.h"
#include "ray.h"
#include "texture.h"

namespace rt {
class Dielectric;
class Lambertian;
class Metal;
using material_t = std::variant<Dielectric, Lambertian, Metal>;

struct Collision;

template<class T>
class Material : public CRTP<Material<T>> {
 public:
  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const {
    return this->Actual().Scatter(ray, collision, attenuation, scattered);
  }

 private:
  Material() = default;
  friend T;
};

class Dielectric : public Material<Dielectric> {
 public:
  explicit Dielectric(float refraction_index);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const;

 private:
  float refraction_index_;

  static float Reflectance(float cosine, float refraction_index);
};

class Lambertian : public Material<Lambertian> {
 public:
  explicit Lambertian(const Texture* albedo);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const;

 private:
  const Texture* albedo_;
};

class Metal : public Material<Metal> {
 public:
  explicit Metal(glm::vec3 albedo, float fuzziness);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const;

 private:
  glm::vec3 albedo_{0, 0, 0};
  float fuzziness_;
};
}  // namespace rt
