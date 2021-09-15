#pragma once

#include <memory>
#include <vector>

#include "vector3d.h"

class Texture {
 public:
  virtual ~Texture() = default;

  [[nodiscard]] virtual Color SampleColor(double u, double v, const Point3D &point) const = 0;
};

class SolidColorTexture : public Texture {
 public:
  explicit SolidColorTexture(Color color);

  SolidColorTexture(double red, double green, double blue);

  [[nodiscard]] Color SampleColor(double u, double v, const Point3D &point) const override;

 private:
  Color color_;
};

class CheckerTexture : public Texture {
 public:
  CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd);

  CheckerTexture(const Color &even, const Color &odd);

  [[nodiscard]] Color SampleColor(double u, double v, const Point3D &point) const override;

 private:
  std::shared_ptr<Texture> even_, odd_;
};
