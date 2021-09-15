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
  * Compute emitted color of a material.
  * If a material doesn't define emission, black is returned.
  * @param[in] u horizontal coordinate
  * @param[in] v vertical coordinate
  * @param[in] point sampling point
  * @return emitted color
  */
  [[nodiscard]] virtual Color Emit(double u, double v, const Point3D &point) const {
    return {0, 0, 0};  // Black as default.
  }

  /**
   * Compute scattered ray and attenuation from an inbound ray and collision information.
   * @param[in] ray inbound ray
   * @param[in] collision contains collision information
   * @param[out] attenuation receives computed attenuation
   * @param[out] scattered_ray receives computed scattered ray
   * @return true if scattering happened, false otherwise
   */
  virtual bool Scatter(const Ray &ray, const Collision &collision, Color &attenuation, Ray &scattered_ray) const {
    return false;
  }
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

class DiffuseLight : public Material {
 public:
  explicit DiffuseLight(const Color &albedo);

  explicit DiffuseLight(std::shared_ptr<Texture> albedo);

  /**
   * Compute emitted color of a diffuse light.
   * @param[in] u horizontal coordinate
   * @param[in] v vertical coordinate
   * @param[in] point sampling point
   * @return emitted color
   */
  [[nodiscard]] Color Emit(double u, double v, const Point3D &point) const override;
 private:
  std::shared_ptr<Texture> emit_;
};

class Isotropic : public Material {
 public:
  explicit Isotropic(std::shared_ptr<Texture> albedo);

  explicit Isotropic(const Color &albedo);

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
  std::shared_ptr<Texture> albedo_;
};
