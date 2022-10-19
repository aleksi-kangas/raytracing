#pragma once

#include "box.h"
#include "rectangle.h"
#include "sphere.h"

namespace rt {
using primitive_t = std::variant<Box, MovingSphere, RectangleXY, RectangleXZ, RectangleYZ, Sphere>;
}  // namespace rt
