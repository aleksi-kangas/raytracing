#pragma once

#include <memory>
#include <vector>

#include "aabb.h"
#include "collidable.h"
#include "ray.h"

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
   * Get the underlying collection of collidable objects.
   * @return vector of collidable objects
   */
  std::vector<std::shared_ptr<Collidable>> CollidableObjects() const;

  /**
   * Compute collision of a ray and the collection of collidable objects.
   * @param ray inbound ray
   * @param t_min minimum threshold
   * @param t_max maximum threshold
   * @param collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

  /**
   * Compute a minimum AABB that surrounds the collection of collidable objects.
   * @param[in] time0 minimum threshold
   * @param[in] time1 maximum threshold
   * @param[out] bounding_box receives the computed AABB
   * @return true if an AABB exists, false otherwise
   */
  bool BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const override;

 private:
  std::vector<std::shared_ptr<Collidable>> objects_;
};
