#pragma once

#include "glm/glm.hpp"

namespace rt::utils {

uint32_t ColorToRGBA(const glm::vec4& color);

bool IsNearZero(const glm::vec3& vec);

}  // namespace rt::utils
