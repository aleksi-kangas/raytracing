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
  [[nodiscard]] std::vector<std::shared_ptr<Collidable>> CollidableObjects() const;

  /**
   * Compute collision of a ray and the collection of collidable objects.
   * @param[in] ray inbound ray
   * @param[in] t_min minimum threshold
   * @param[in] t_max maximum threshold
   * @param[out] collision receives collision information
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

  /**
   * Compute value of probability density function given a point and direction.
   * @param[in] origin point
   * @param[in] vector direction
   * @return value of probability density function
   */
  [[nodiscard]] double PDFValue(const Point3D &origin, const Vector3D &vector) const override;

  /**
   * Generate a Random direction towards the collection of collidable objects from the given point.
   * @param[in] origin point
   * @return random direction
   */
  [[nodiscard]] Vector3D Random(const Point3D &origin) const override;

 private:
  std::vector<std::shared_ptr<Collidable>> objects_;
};
