#pragma once

#include <memory>
#include <vector>

#include "collidable.h"

/**
 * Represents a collection of collidable objects that form the world of a scene.
 */
class Collidables : public Collidable {
 public:
  /**
   * Add a collidable object to the collection.
   * @param object collidable object
   */
  void AddCollidable(const std::shared_ptr<Collidable> &object);

  /**
   * Empty the collection of collidable objects.
   */
  void Clear();

  /**
   * Compute collision of a ray and the collection of collidable objects.
   * @param ray inbound ray
   * @param t_min minimum threshold
   * @param t_max maximum threshold
   * @param collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

 private:
  std::vector<std::shared_ptr<Collidable>> objects_;
};
