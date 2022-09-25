#include "random.h"

#include <random>

namespace rt::random {
static std::mt19937 generator{std::random_device{}()};
static std::uniform_int_distribution<std::mt19937::result_type> distribution;

uint32_t UInt32() {
  return distribution(generator);
}

uint32_t UInt32(uint32_t min, uint32_t max) {
  return min + distribution(generator) % (max - min + 1);
}

}  // namespace rt::random
