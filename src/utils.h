#include <array>
#include <cmath>
#include <filesystem>
#include <limits>
#include <vector>

#include "vector3d.h"

namespace utils {
constexpr double kInfinity = std::numeric_limits<double>::max();
constexpr double kNegativeInfinity = std::numeric_limits<double>::lowest();
const double kPI = std::atan(1.0) * 4;

std::array<unsigned char, 3> ColorToRGB(const Color &color, int samples_per_pixel);

/**
 * Convert degrees into radians.
 * @param degrees to convert
 * @return radians
 */
double DegreesToRadians(double degrees);

/**
 * Read a .png-file from disk.
 * @param[in] path filepath
 * @param[out] width receives image pixel width
 * @param[out] height receives image pixel height
 * @return 2D matrix of image pixels
 */
std::vector<std::vector<Color>> ReadPNG(const std::filesystem::path &path, unsigned int &width, unsigned int &height);

/**
 * Write contents of the given image buffer to a .png-file.
 * @param[in] path filepath
 * @param[in] width image pixel width
 * @param[in] height image pixel height
 * @param[in] samples_per_pixel how many samples per pixel were used in rendering
 * @param[in] image contains color values ([0, 1]) for each pixel
 */
void WritePNG(const std::filesystem::path &path,
              int width,
              int height,
              int samples_per_pixel,
              const std::vector<std::vector<Color>> &image);
}  // namespace utils
