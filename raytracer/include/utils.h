#pragma once

#include <cstdint>
#include <filesystem>

#include "glm/glm.hpp"

#include "image.h"

namespace rt::utils {

uint32_t ColorToRGBA(const glm::vec4& color);

bool IsNearZero(const glm::vec3& vec);

namespace png {

std::vector<uint8_t> Decode(const std::filesystem::path& path, int32_t& width, int32_t& height);

void Encode(const std::filesystem::path& path, const Image& image);

}  // namespace rt::utils::png

}  // namespace rt::utils
