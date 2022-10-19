#pragma once

#include <variant>

#include "constant_medium.h"
#include "primitives.h"
#include "flip.h"

namespace rt {
using transformable_t = std::variant<Box,
                                     ConstantMedium,
                                     Flip,
                                     MovingSphere,
                                     RectangleXY,
                                     RectangleXZ,
                                     RectangleYZ,
                                     Sphere>;
}  // namespace rt
