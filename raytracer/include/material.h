#pragma once

#include <variant>
#include <vector>

#include "glm/glm.hpp"

#include "crtp.h"
#include "ray.h"
#include "texture.h"

namespace rt {
class Dielectric;
class DiffuseLight;
class Isotropic;
class Lambertian;
class Metal;
using material_t = std::variant<Dielectric, DiffuseLight, Isotropic, Lambertian, Metal>;

struct Collision;

template<class T>
class Material : public CRTP<Material<T>> {
 public:
  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const {
    return this->Actual().Scatter(ray, collision, attenuation, scattered);
  }

  [[nodiscard]] glm::vec3 Emit(float u, float v, const glm::vec3& point) const {
    return this->Actual().Emit(u, v, point);
  }

 private:
  Material() = default;
  friend T;
};

class DiffuseLight : public Material<DiffuseLight> {
 public:
  explicit DiffuseLight(glm::vec3 emit);
  explicit DiffuseLight(texture_t emit);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(float u, float v, const glm::vec3& point) const;

 private:
  texture_t emit_;
};

class Dielectric : public Material<Dielectric> {
 public:
  explicit Dielectric(float refraction_index);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(float u, float v, const glm::vec3& point) const;

 private:
  float refraction_index_;

  static float Reflectance(float cosine, float refraction_index);
};

class Isotropic : public Material<Isotropic> {
 public:
  explicit Isotropic(glm::vec3 albedo);
  explicit Isotropic(texture_t albedo);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(float u, float v, const glm::vec3& point) const;

 private:
  texture_t albedo_;
};

class Lambertian : public Material<Lambertian> {
 public:
  explicit Lambertian(texture_t albedo);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(float u, float v, const glm::vec3& point) const;

 private:
  texture_t albedo_;
};

class Metal : public Material<Metal> {
 public:
  explicit Metal(glm::vec3 albedo, float fuzziness);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(float u, float v, const glm::vec3& point) const;

 private:
  glm::vec3 albedo_{0, 0, 0};
  float fuzziness_;
};
}  // namespace rt
