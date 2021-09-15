#pragma once

#include <memory>

#include "collidable.h"
#include "ray.h"
#include "texture.h"
#include "vector3d.h"

/**
 * Represents the material of an object in 3D space.
 */
class Material {
 public:
  virtual ~Material() = default;

  /**
   * Compute scattered ray and attenuation from an inbound ray and collision information.
   * @param[in] ray inbound ray
   * @param[in] collision contains collision information
   * @param[out] attenuation receives computed attenuation
   * @param[out] scattered_ray receives computed scattered ray
   * @return true if scattering happened, false otherwise
   */
  virtual bool Scatter(const Ray &ray, const Collision &collision, Color &attenuation, Ray &scattered_ray) const = 0;
};

class Lambertian : public Material {
 public:
  explicit Lambertian(const Color &albedo);

  explicit Lambertian(std::shared_ptr<Texture> albedo);

  /**
   * Compute scattered ray and attenuation from an inbound ray and collision information.
   * @param[in] ray inbound ray
   * @param[in] collision contains collision information
   * @param[out] attenuation receives computed attenuation
   * @param[out] scattered_ray receives computed scattered ray
   * @return true if scattering happened, false otherwise
   */
  bool Scatter(const Ray &ray,
               const Collision &collision,
               Color &attenuation,
               Ray &scattered_ray) const override;

 private:
  std::shared_ptr<Texture> albedo_;
};

/**
 * Reflective metal material.
 */
class Metal : public Material {
 public:
  Metal(const Color &albedo, double fuzziness);

  /**
   * Compute scattered ray and attenuation from an inbound ray and collision information.
   * @param[in] ray inbound ray
   * @param[in] collision contains collision information
   * @param[out] attenuation receives computed attenuation
   * @param[out] scattered_ray receives computed scattered ray
   * @return true if scattering happened, false otherwise
   */
  bool Scatter(const Ray &ray, const Collision &collision, Color &attenuation, Ray &scattered_ray) const override;

 private:
  Color albedo_;
  double fuzziness_;
};

/**
 * Refractive glass material.
 */
class Dielectric : public Material {
 public:
  explicit Dielectric(double refraction_ratio);

  /**
   * Compute scattered ray and attenuation from an inbound ray and collision information.
   * @param[in] ray inbound ray
   * @param[in] collision contains collision information
   * @param[out] attenuation receives computed attenuation
   * @param[out] scattered_ray receives computed scattered ray
   * @return true if scattering happened, false otherwise
   */
  bool Scatter(const Ray &ray, const Collision &collision, Color &attenuation, Ray &scattered_ray) const override;

 private:
  double refraction_ratio_;

  static double ComputeReflectance(double cosine, double refraction_ratio);
};
