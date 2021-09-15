#pragma once

#include <array>
#include <memory>

#include "collidable.h"
#include "onb.h"
#include "vector3d.h"

class ProbabilityDensityFunction {
 public:
  [[nodiscard]] virtual double Value(const Vector3D &direction) const = 0;

  [[nodiscard]] virtual Vector3D Generate() const = 0;
};

class CosinePDF : public ProbabilityDensityFunction {
 public:
  explicit CosinePDF(const Vector3D &w);

  [[nodiscard]] double Value(const Vector3D &direction) const override;

  [[nodiscard]] Vector3D Generate() const override;

 private:
  OrthonormalBasis onb_;
};

class CollidablePDF : public ProbabilityDensityFunction {
 public:
  CollidablePDF(std::shared_ptr<Collidable> object, const Point3D &origin);

  [[nodiscard]] double Value(const Vector3D &direction) const override;

  [[nodiscard]] Vector3D Generate() const override;

 private:
  std::shared_ptr<Collidable> object_;
  Point3D origin_;
};

class MixturePDF : public ProbabilityDensityFunction {
 public:
  MixturePDF(std::shared_ptr<ProbabilityDensityFunction> pdf0, std::shared_ptr<ProbabilityDensityFunction> pdf1);

  [[nodiscard]] double Value(const Vector3D &direction) const override;

  [[nodiscard]] Vector3D Generate() const override;

 private:
  std::array<std::shared_ptr<ProbabilityDensityFunction>, 2> pdfs_;
};
