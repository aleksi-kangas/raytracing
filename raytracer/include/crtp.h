#pragma once

namespace rt {
/**
 * Helper class for applying the Curiously Recurring Template Pattern (CRTP) for compile time polymorphism (CTP).
 * https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 * @tparam T Class
 */
template<class T>
class CRTP {
 public:
  T& Actual() { return static_cast<T&>(*this); }
  const T& Actual() const { return static_cast<const T&>(*this); }
 private:
  CRTP() = default;
  friend T;
};

// Primitives
class Box;
class MovingSphere;
class RectangleXY;
class RectangleXZ;
class RectangleYZ;
class Sphere;
using primitive_t = std::variant<Box, MovingSphere, RectangleXY, RectangleXZ, RectangleYZ, Sphere>;

class ConstantMedium;
class Flip;
using transformable_t = std::variant<Box,
                                     ConstantMedium,
                                     Flip,
                                     MovingSphere,
                                     RectangleXY,
                                     RectangleXZ,
                                     RectangleYZ,
                                     Sphere>;

class RotateTranslate;
using collidable_t = std::variant<Box,
                                  ConstantMedium,
                                  Flip,
                                  MovingSphere,
                                  RectangleXY,
                                  RectangleXZ,
                                  RectangleYZ,
                                  RotateTranslate,
                                  Sphere>;
using collidable_container_t = std::vector<collidable_t>;

// Materials
class Dielectric;
class DiffuseLight;
class Isotropic;
class Lambertian;
class Metal;
using material_t = std::variant<Dielectric, DiffuseLight, Isotropic, Lambertian, Metal>;

// PDFs
class CosinePDF;
class CollidablePDF;
template<class T1, class T2>
class MixturePDF;
using pdf_t = std::variant<CosinePDF, CollidablePDF, MixturePDF<CosinePDF, CollidablePDF>>;

// Textures
class SolidColorTexture;
class CheckerTexture;
class NoiseTexture;
class ImageTexture;
using texture_t = std::variant<SolidColorTexture, CheckerTexture, NoiseTexture, ImageTexture>;
}  // namespace rt
