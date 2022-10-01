#include "perlin.h"

#include "random.h"

namespace rt {
Perlin::Perlin() {
  for (uint32_t i = 0; i < kPointCount; ++i) {
    random_vectors_[i] = random::Vec3(-1.0f, 1.0f);
    permutation_x_[i] = i;
    permutation_y_[i] = i;
    permutation_z_[i] = i;
  }
  Permute(permutation_x_);
  Permute(permutation_y_);
  Permute(permutation_z_);
}

float Perlin::Noise(const glm::vec3& point) const {
  glm::vec3 uvw = point - glm::floor(point);
  const auto i = static_cast<int32_t>(floor(point.x));
  const auto j = static_cast<int32_t>(floor(point.y));
  const auto k = static_cast<int32_t>(floor(point.z));
  glm::vec3 c[2][2][2];
  for (int32_t di = 0; di < 2; ++di) {
    for (int32_t dj = 0; dj < 2; ++dj) {
      for (int32_t dk = 0; dk < 2; ++dk) {
        c[di][dj][dk] = random_vectors_[
            permutation_x_[(i + di) & 255] ^ permutation_y_[(j + dj) & 255] ^ permutation_z_[(k + dk) & 255]];
      }
    }
  }
  return Interpolation(c, uvw);
}

float Perlin::Turbulence(glm::vec3 point, uint32_t depth) const {
  float accumulated = 0.0f;
  float weight = 1.0f;
  for (uint32_t i = 0; i < depth; ++i) {
    accumulated += weight * Noise(point);
    weight *= 0.5f;
    point *= 2.0f;
  }
  return fabs(accumulated);
}

void Perlin::Permute(std::array<uint32_t, kPointCount>& permutation) {
  // TODO Use std for permutation?
  for (uint32_t i = kPointCount - 1; i > 0; --i) {
    const uint32_t target = random::UInt32(0, i);
    std::swap(permutation[i], permutation[target]);
  }
}

float Perlin::Interpolation(glm::vec3 c[2][2][2], glm::vec3 uvw) {
  uvw = uvw * uvw * (3.0f - 2.0f * uvw);
  float accumulated = 0.0f;
  for (int32_t i = 0; i < 2; ++i) {
    for (int32_t j = 0; j < 2; ++j) {
      for (int32_t k = 0; k < 2; ++k) {
        const glm::vec3 weight{uvw.x - static_cast<float>(i),
                               uvw.y - static_cast<float>(j),
                               uvw.z - static_cast<float>(k)};
        accumulated += glm::dot(c[i][j][k], weight) *
            (static_cast<float>(i) * uvw.x + (1.0f - static_cast<float>(i)) * (1.0f - uvw.x)) *
            (static_cast<float>(j) * uvw.y + (1.0f - static_cast<float>(j)) * (1.0f - uvw.y)) *
            (static_cast<float>(k) * uvw.z + (1.0f - static_cast<float>(k)) * (1.0f - uvw.z));
      }
    }
  }
  return accumulated;
}

}  // namespace rt
