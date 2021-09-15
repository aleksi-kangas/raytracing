#include "perlin.h"

#include <cmath>

#include "random.h"

Perlin::Perlin() {
  for (int i = 0; i < kPointCount; ++i) {
    random_vectors_[i] = Vector3D::Random(-1, 1).UnitVector();
  }
  permutations_[0] = GeneratePerlinPermutation();
  permutations_[1] = GeneratePerlinPermutation();
  permutations_[2] = GeneratePerlinPermutation();
}

double Perlin::Noise(const Point3D &point) const {
  double u = point.X() - floor(point.X());
  double v = point.Y() - floor(point.Y());
  double w = point.Z() - floor(point.Z());

  u = u * u * (3 - 2 * u);
  v = v * v * (3 - 2 * v);
  w = w * w * (3 - 2 * w);

  int i = static_cast<int>(floor(point.X()));
  int j = static_cast<int>(floor(point.Y()));
  int k = static_cast<int>(floor(point.Z()));

  Vector3D c[2][2][2];
  for (int di = 0; di < 2; ++di) {
    for (int dj = 0; dj < 2; ++dj) {
      for (int dk = 0; dk < 2; ++dk) {
        c[di][dj][dk] = random_vectors_[permutations_[0][(i + di) & 255] ^ permutations_[1][(j + dj) & 255]
            ^ permutations_[2][(k + dk) & 255]];
      }
    }
  }

  return PerlinInterpolation(c, u, v, w);
}

double Perlin::Turbulence(const Point3D &point, int depth) const {
  double accumulator = 0.0;
  Point3D temp_point = point;
  double weight = 1.0;

  for (int i = 0; i < depth; ++i) {
    accumulator += weight * Noise(temp_point);
    weight *= 0.5;
    temp_point *= 2.0;
  }
  return std::abs(accumulator);
}

std::array<int, Perlin::kPointCount> Perlin::GeneratePerlinPermutation() {
  std::array<int, kPointCount> permutation{};
  for (int i = 0; i < kPointCount; ++i) {
    permutation[i] = i;
  }

  for (int i = kPointCount - 1; i > 0; --i) {
    int target = RandomInt(0, i);
    std::swap(permutation[i], permutation[target]);
  }
  return permutation;
}

double Perlin::TriLinearInterpolation(double c[2][2][2], double u, double v, double w) {
  double accumulator = 0.0;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 2; ++k) {
        accumulator +=
            c[i][j][k] *
                (i * u + (1.0 - i) * (1.0 - u)) *
                (j * v + (1.0 - j) * (1.0 - v)) *
                (k * w + (1.0 - k) * (1.0 - w));
      }
    }
  }
  return accumulator;
}

double Perlin::PerlinInterpolation(Vector3D c[2][2][2], double u, double v, double w) {
  double accumulator = 0.0;

  double uu = u * u * (3 - 2 * u);
  double vv = v * v * (3 - 2 * v);
  double ww = w * w * (3 - 2 * w);

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 2; ++k) {
        Vector3D weight(u - i, v - j, w - k);
        accumulator +=
            (i * uu + (1.0 - i) * (1.0 - uu)) * (j * vv + (1.0 - j) * (1.0 - vv)) * (k * ww + (1.0 - k) * (1.0 - ww))
                * Vector3D::DotProduct(c[i][j][k], weight);
      }
    }
  }
  return accumulator;
}
