#pragma once

#include "collidables.h"

/**
 * Represents a scene to be rendered.
 */
struct Scene {
  Scene(int image_width, int image_height, int samples_per_pixel);

  /**
   * Initialize the world of the scene.
   */
  void InitializeWorld();

  int image_width = 0;
  int image_height = 0;
  int samples_per_pixel = 0;
  Collidables world;
};
