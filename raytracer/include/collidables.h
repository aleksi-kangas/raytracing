#pragma once

#include <variant>

#include "transformables.h"
#include "transform.h"

namespace rt {
using collidable_t = std::variant<Box,
                                  ConstantMedium,
                                  Flip,
                                  MovingSphere,
                                  RectangleXY,
                                  RectangleXZ,
                                  RectangleYZ,
                                  Transform,
                                  Sphere>;
using collidable_container_t = std::vector<collidable_t>;
}  // namespace rt
