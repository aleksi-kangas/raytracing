#pragma once

#include <array>

#include "vector3d.h"

class OrthonormalBasis {
 public:
  explicit OrthonormalBasis(const Vector3D &w);

  Vector3D operator[](int i) const;

  [[nodiscard]] Vector3D U() const;
  [[nodiscard]] Vector3D V() const;
  [[nodiscard]] Vector3D W() const;

  [[nodiscard]] Vector3D Local(double a, double b, double c) const;

  [[nodiscard]] Vector3D Local(const Vector3D &a) const;

 private:
  std::array<Vector3D, 3> axes_;
};
