#include "random.h"

#include <random>

namespace rt::random {

uint32_t UInt32() {
  static thread_local std::mt19937 generator{std::random_device{}()};
  static std::uniform_int_distribution<uint32_t> distribution{0, 1};
  return distribution(generator);
}

uint32_t UInt32(uint32_t min, uint32_t max) {
  static thread_local std::mt19937 generator{std::random_device{}()};
  std::uniform_int_distribution<uint32_t> distribution{min, max};
  return distribution(generator);
}

float Float() {
  static thread_local std::mt19937 generator{std::random_device{}()};
  static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
  return distribution(generator);
}

float Float(float min, float max) {
  static thread_local std::mt19937 generator{std::random_device{}()};
  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(generator);
}

glm::vec3 Vec3() {
  return {Float(), Float(), Float()};
}

glm::vec3 Vec3(float min, float max) {
  return {Float(min, max), Float(min, max), Float(min, max)};
}

glm::vec3 InUnitSphere() {
  while (true) {
    auto p = Vec3(-1.0f, 1.0f);
    if (glm::dot(p, p) >= 1.0f) continue;
    return p;
  }
}

glm::vec3 InUnitDisk() {
  while (true) {
    auto p = glm::vec3{Float(-1.0f, 1.0f), Float(-1.0f, 1.0f), 0.0f};
    if (glm::dot(p, p) >= 1.0f) continue;
    return p;
  }
}

glm::vec3 UnitVec3() {
  return glm::normalize(InUnitSphere());
}

}  // namespace rt::random
