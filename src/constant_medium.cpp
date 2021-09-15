#include "constant_medium.h"

#include <utility>

#include "random.h"
#include "utils.h"

ConstantMedium::ConstantMedium(std::shared_ptr<Collidable> boundary, double density, std::shared_ptr<Texture> albedo)
    : boundary_(std::move(boundary)), density_(density), phase_function_(std::make_shared<Isotropic>(albedo)) {}

ConstantMedium::ConstantMedium(std::shared_ptr<Collidable> boundary, double density, Color albedo)
    : boundary_(std::move(boundary)), density_(density), phase_function_(std::make_shared<Isotropic>(albedo)) {}

bool ConstantMedium::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  Collision collision1, collision2;

  if (!boundary_->Collide(ray, utils::kNegativeInfinity, utils::kInfinity, collision1)) {
    return false;
  }
  if (!boundary_->Collide(ray, collision1.t + 0.0001, utils::kInfinity, collision2)) {
    return false;
  }

  collision1.t = std::max(collision1.t, t_min);
  collision2.t = std::min(collision2.t, t_max);

  if (collision1.t >= collision2.t) {
    return false;
  }

  collision1.t = std::max(collision1.t, 0.0);

  double ray_length = ray.Direction().Length();
  double distance_inside_boundary = (collision2.t - collision1.t) * ray_length;
  double collision_distance = (-1.0 / density_) * log(RandomDouble());

  if (collision_distance > distance_inside_boundary) {
    return false;
  }

  collision.t = collision1.t + collision_distance / ray_length;
  collision.point = ray.PointAt(collision.t);
  collision.material = phase_function_.get();
  return true;
}

bool ConstantMedium::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  return boundary_->BoundingBox(time0, time1, bounding_box);
}
