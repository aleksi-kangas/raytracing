#pragma once

#include <cstdint>

namespace rt::random {
uint32_t UInt32();

uint32_t UInt32(uint32_t min, uint32_t max);

float Float();

float Float(float min, float max);

}  // namespace rt::random
