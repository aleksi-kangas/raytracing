#pragma once

/**
 * Represents a scene to be rendered.
 */
struct Scene {
  Scene(int image_width, int image_height, int samples_per_pixel);

  int image_width = 0;
  int image_height = 0;
  int samples_per_pixel = 0;
};
