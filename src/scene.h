#pragma once

#include "camera.h"
#include "collidables.h"

/**
 * Represents a scene to be rendered.
 */
struct Scene {
  Scene(int image_width, int image_height, int samples_per_pixel);

  /**
   * Initialize the camera for rendering.
   */
  void InitializeCamera();

  /**
   * Initialize the world of the scene.
   */
  void InitializeWorld();

  int image_width = 0;
  int image_height = 0;
  int samples_per_pixel = 0;
  static constexpr int kMaxChildRays = 50;
  Color background_color{0, 0, 0};
  std::unique_ptr<Camera> camera = nullptr;
  Collidables world;
};
