#include "random.h"

#include <random>

double RandomDouble() {
  static thread_local std::mt19937 generator(std::random_device{}());
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  return distribution(generator);
}

double RandomDouble(double min, double max) {
  static thread_local std::mt19937 generator(std::random_device{}());
  std::uniform_real_distribution<double> distribution(min, max);
  return distribution(generator);
}

int RandomInt(int min, int max) {
  return static_cast<int>(RandomDouble(min, max + 1.0));
}
