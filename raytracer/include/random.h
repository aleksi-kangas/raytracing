#pragma once

#include <cstdint>

#include "glm/glm.hpp"

namespace rt::random {
uint32_t UInt32();

uint32_t UInt32(uint32_t min, uint32_t max);

float Float();

float Float(float min, float max);

glm::vec3 Vec3();

glm::vec3 Vec3(float min, float max);

glm::vec3 CosineDirection();

glm::vec3 InUnitSphere();

glm::vec3 InUnitDisk();

glm::vec3 InHemisphere(const glm::vec3& normal);

glm::vec3 UnitVec3();

}  // namespace rt::random
