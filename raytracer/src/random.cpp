#include "random.h"

#include <random>

namespace rt::random {
static std::mt19937 generator{std::random_device{}()};

uint32_t UInt32() {
  static std::uniform_int_distribution<uint32_t> distribution;
  return distribution(generator);
}

uint32_t UInt32(uint32_t min, uint32_t max) {
  std::uniform_int_distribution<uint32_t> distribution{min, max};
  return distribution(generator);
}

float Float() {
  static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
  return distribution(generator);
}

float Float(float min, float max) {
  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(generator);
}

}  // namespace rt::random
