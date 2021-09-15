
#pragma once

#include <array>

#include "vector3d.h"

class Perlin {
 public:
  Perlin();

  [[nodiscard]] double Noise(const Point3D &point) const;

  [[nodiscard]] double Turbulence(const Point3D &point, int depth = 7) const;

 private:
  static constexpr int kPointCount = 256;
  std::array<Vector3D, kPointCount> random_vectors_{};
  std::array<int, kPointCount> permutations_[3]{};

  static std::array<int, kPointCount> GeneratePerlinPermutation();

  static double TriLinearInterpolation(double c[2][2][2], double u, double v, double w);

  static double PerlinInterpolation(Vector3D c[2][2][2], double u, double v, double w);
};
