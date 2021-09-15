#pragma once

#include <memory>

#include "aabb.h"
#include "collidable.h"
#include "material.h"
#include "ray.h"
#include "vector3d.h"

class RectangleXY : public Collidable {
 public:
  RectangleXY(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> material);

  /**
   * Compute collision of a ray and the rectangle object.
   * @param[in] ray inbound ray
   * @param[in] t_min minimum threshold
   * @param[in] t_max maximum threshold
   * @param[out] collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

  /**
   * Compute a minimum AABB that surrounds the sphere object.
   * @param[in] time0 minimum threshold
   * @param[in] time1 maximum threshold
   * @param[out] bounding_box receives the computed AABB
   * @return true if an AABB exists, false otherwise
   */
  bool BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const override;

 private:
  double x0_, x1_, y0_, y1_, k_;
  std::shared_ptr<Material> material_;
};

class RectangleXZ : public Collidable {
 public:
  RectangleXZ(double x0, double x1, double z0, double z1, double k, std::shared_ptr<Material> material);

  /**
   * Compute collision of a ray and the rectangle object.
   * @param[in] ray inbound ray
   * @param[in] t_min minimum threshold
   * @param[in] t_max maximum threshold
   * @param[out] collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

  /**
   * Compute a minimum AABB that surrounds the sphere object.
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
   * Generate a Random direction towards the collidable object from the given point.
   * @param[in] origin point
   * @return random direction
   */
  [[nodiscard]] Vector3D Random(const Point3D &origin) const override;

 private:
  double x0_, x1_, z0_, z1_, k_;
  std::shared_ptr<Material> material_;
};

class RectangleYZ : public Collidable {
 public:
  RectangleYZ(double y0, double y1, double z0, double z1, double k, std::shared_ptr<Material> material);

  /**
   * Compute collision of a ray and the rectangle object.
   * @param[in] ray inbound ray
   * @param[in] t_min minimum threshold
   * @param[in] t_max maximum threshold
   * @param[out] collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

  /**
   * Compute a minimum AABB that surrounds the sphere object.
   * @param[in] time0 minimum threshold
   * @param[in] time1 maximum threshold
   * @param[out] bounding_box receives the computed AABB
   * @return true if an AABB exists, false otherwise
   */
  bool BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const override;

 private:
  double y0_, y1_, z0_, z1_, k_;
  std::shared_ptr<Material> material_;
};
