#include "rectangle.h"

#include <utility>

#include "random.h"
#include "utils.h"

RectangleXY::RectangleXY(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> material)
    : x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), material_(std::move(material)) {}

bool RectangleXY::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  double t = (k_ - ray.Origin().Z()) / ray.Direction().Z();
  if (t < t_min || t > t_max) {
    return false;
  }
  double x = ray.Origin().X() + t * ray.Direction().X();
  double y = ray.Origin().Y() + t * ray.Direction().Y();
  if (x < x0_ || x > x1_ || y < y0_ || y > y1_) {
    return false;
  }
  collision.u = (x - x0_) / (x1_ - x0_);
  collision.v = (y - y0_) / (y1_ - y0_);
  collision.t = t;
  Vector3D outward_normal{0, 0, 1};
  collision.SetFaceNormal(ray, outward_normal);
  collision.material = material_.get();
  collision.point = ray.PointAt(t);
  return true;
}

bool RectangleXY::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  bounding_box = AxisAlignedBoundingBox(
      Point3D(x0_, y0_, k_ - 0.0001),
      Point3D(x1_, y1_, k_ + 0.0001)
  );
  return true;
}

RectangleXZ::RectangleXZ(double x0, double x1, double z0, double z1, double k, std::shared_ptr<Material> material)
    : x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), material_(std::move(material)) {}

bool RectangleXZ::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  double t = (k_ - ray.Origin().Y()) / ray.Direction().Y();
  if (t < t_min || t > t_max) {
    return false;
  }
  double x = ray.Origin().X() + t * ray.Direction().X();
  double z = ray.Origin().Z() + t * ray.Direction().Z();
  if (x < x0_ || x > x1_ || z < z0_ || z > z1_) {
    return false;
  }
  collision.u = (x - x0_) / (x1_ - x0_);
  collision.v = (z - z0_) / (z1_ - z0_);
  collision.t = t;
  Vector3D outward_normal{0, 1, 0};
  collision.SetFaceNormal(ray, outward_normal);
  collision.material = material_.get();
  collision.point = ray.PointAt(t);
  return true;
}

bool RectangleXZ::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  bounding_box = AxisAlignedBoundingBox(
      Point3D(x0_, k_ - 0.0001, z0_),
      Point3D(x1_, k_ + 0.0001, z1_)
  );
  return true;
}

double RectangleXZ::PDFValue(const Point3D &origin, const Vector3D &vector) const {
  Collision collision;
  if (!Collide(Ray(origin, vector), 0.001, utils::kInfinity, collision)) {
    return 0.0;
  }
  double area = (x1_ - x0_) * (z1_ - z0_);
  double distance_squared = collision.t * collision.t * vector.LengthSquared();
  double cosine = std::abs(Vector3D::DotProduct(vector, collision.normal) / vector.Length());
  return distance_squared / (cosine * area);
}

Vector3D RectangleXZ::Random(const Point3D &origin) const {
  Point3D random_point = Point3D(RandomDouble(x0_, x1_), k_, RandomDouble(z0_, z1_));
  return random_point - origin;
}

RectangleYZ::RectangleYZ(double y0, double y1, double z0, double z1, double k, std::shared_ptr<Material> material)
    : y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k), material_(std::move(material)) {}

bool RectangleYZ::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  double t = (k_ - ray.Origin().X()) / ray.Direction().X();
  if (t < t_min || t > t_max) {
    return false;
  }
  double y = ray.Origin().Y() + t * ray.Direction().Y();
  double z = ray.Origin().Z() + t * ray.Direction().Z();
  if (y < y0_ || y > y1_ || z < z0_ || z > z1_) {
    return false;
  }
  collision.u = (y - y0_) / (y1_ - y0_);
  collision.v = (z - z0_) / (z1_ - z0_);
  collision.t = t;
  Vector3D outward_normal{1, 0, 0};
  collision.SetFaceNormal(ray, outward_normal);
  collision.material = material_.get();
  collision.point = ray.PointAt(t);
  return true;
}

bool RectangleYZ::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  bounding_box = AxisAlignedBoundingBox(
      Point3D(k_ - 0.0001, y0_, z0_),
      Point3D(k_ + 0.0001, y1_, z1_)
  );
  return true;
}
