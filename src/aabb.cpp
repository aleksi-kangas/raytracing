#include "aabb.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Point3D &min_point, const Point3D &max_point)
    : min_point_(min_point), max_point_(max_point) {}

Point3D AxisAlignedBoundingBox::MinPoint() const {
  return min_point_;
}

Point3D AxisAlignedBoundingBox::MaxPoint() const {
  return max_point_;
}

bool AxisAlignedBoundingBox::Collide(const Ray &ray, double t_min, double t_max) const {
  // Awesome optimization trick by Roman Wiche (and C++ implementation Benjamin Summerton).
  // Provides a nice boost to collision calculation.
  // Roman Wiche: https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
  // Benjamin Summerton: https://github.com/define-private-public/PSRayTracing/blob/52fa79a5b1978deffe1119338e59d26b283072c0/src/AABB.cpp#L71

  double t0[3] = {
      (min_point_.X() - ray.Origin().X()) / ray.Direction().X(),
      (min_point_.Y() - ray.Origin().Y()) / ray.Direction().Y(),
      (min_point_.Z() - ray.Origin().Z()) / ray.Direction().Z()
  };

  double t1[3] = {
      (max_point_.X() - ray.Origin().X()) / ray.Direction().X(),
      (max_point_.Y() - ray.Origin().Y()) / ray.Direction().Y(),
      (max_point_.Z() - ray.Origin().Z()) / ray.Direction().Z()
  };

  double smaller[3] = {
      std::min(t0[0], t1[0]),
      std::min(t0[1], t1[1]),
      std::min(t0[2], t1[2])
  };
  double larger[3] = {
      std::max(t0[0], t1[0]),
      std::max(t0[1], t1[1]),
      std::max(t0[2], t1[2])
  };

  t_min = std::max(t_min, std::max(smaller[0], std::max(smaller[1], smaller[2])));
  t_max = std::min(t_max, std::min(larger[0], std::min(larger[1], larger[2])));

  return t_min < t_max;
}

AxisAlignedBoundingBox AxisAlignedBoundingBox::SurroundingBox(const AxisAlignedBoundingBox &box0,
                                                              const AxisAlignedBoundingBox &box1) {
  Point3D minimum(
      std::min(box0.MinPoint().X(), box1.MinPoint().X()),
      std::min(box0.MinPoint().Y(), box1.MinPoint().Y()),
      std::min(box0.MinPoint().Z(), box1.MinPoint().Z())
  );
  Point3D maximum(
      std::max(box0.MaxPoint().X(), box1.MaxPoint().X()),
      std::max(box0.MaxPoint().Y(), box1.MaxPoint().Y()),
      std::max(box0.MaxPoint().Z(), box1.MaxPoint().Z())
  );
  return {minimum, maximum};
}
