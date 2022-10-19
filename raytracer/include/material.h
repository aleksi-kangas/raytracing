#pragma once

#include <variant>
#include <vector>

#include "glm/glm.hpp"

#include "crtp.h"
#include "ray.h"
#include "texture.h"

namespace rt {

struct Collision;

template<class T>
class Material : public CRTP<Material<T>> {
 public:
  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered, float& pdf) const {
    return this->Actual().Scatter(ray, collision, attenuation, scattered, pdf);
  }

  [[nodiscard]] float ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const {
    return this->Actual().ScatteringPDF(ray, collision, scattered);
  }

  [[nodiscard]] glm::vec3 Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const {
    return this->Actual().Emit(ray, collision, u, v);
  }

 private:
  Material() = default;
  friend T;
};

class Dielectric : public Material<Dielectric> {
 public:
  explicit Dielectric(float refraction_index);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered, float& pdf) const;

  [[nodiscard]] float ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const;

 private:
  float refraction_index_;

  static float Reflectance(float cosine, float refraction_index);
};

class DiffuseLight : public Material<DiffuseLight> {
 public:
  explicit DiffuseLight(glm::vec3 emit);
  explicit DiffuseLight(texture_t emit);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered, float& pdf) const;

  [[nodiscard]] float ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const;

 private:
  texture_t emit_;
};

class Isotropic : public Material<Isotropic> {
 public:
  explicit Isotropic(glm::vec3 albedo);
  explicit Isotropic(texture_t albedo);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered, float& pdf) const;

  [[nodiscard]] float ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const;

 private:
  texture_t albedo_;
};

class Lambertian : public Material<Lambertian> {
 public:
  explicit Lambertian(texture_t albedo);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered, float& pdf) const;

  [[nodiscard]] float ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const;

 private:
  texture_t albedo_;
};

class Metal : public Material<Metal> {
 public:
  explicit Metal(glm::vec3 albedo, float fuzziness);

  bool Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered, float& pdf) const;

  [[nodiscard]] float ScatteringPDF(const Ray& ray, const Collision& collision, const Ray& scattered) const;

  [[nodiscard]] glm::vec3 Emit(const Ray& ray, const Collision& collision, float u, float v, const glm::vec3& point) const;

 private:
  glm::vec3 albedo_{0, 0, 0};
  float fuzziness_;
};

using material_t = std::variant<Dielectric, DiffuseLight, Isotropic, Lambertian, Metal>;
}  // namespace rt
