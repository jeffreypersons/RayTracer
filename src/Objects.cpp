#include "Objects.h"
#include "Math.hpp"
#include "Ray.hpp"
#include <assert.h>


Sphere::Sphere(const Vec3& center, float radius, const Material& material) {
    // note that base class members can only be initialized in the body and not our typical initializer list
    assert(radius > 0.00f);
    this->radius_ = radius;
    this->center_ = center;
    this->position_ = center;
    this->material_ = material;
}

// given ray [X(t) = P + t] intersects sphere [| X � C | = r] IFF there exists
// at least one real number t [t = - D.M +/- sqrt((D.M)^2 � ( |M|^2 � r^2 ))]
// (that is, if above discriminant >= 0, then there exists a point along ray that also lies along sphere's surface)
bool Sphere::intersect(const Ray& ray, Intersection& result) const {
    const Vec3 M = ray.origin - this->position_;
    const float dDotM = Math::dot(ray.direction, M);
    const float discriminant = Math::square(dDotM) - (Math::magnitudeSquared(M) - Math::square(radius_));
    if (discriminant < 0.00f) {
        return false;
    }

    // we have an intersection, calculate our t, and plug it in to find our normal/intersect-point
    float closestT;
    if (Math::isApproximately(discriminant, 0.00f)) {
        closestT = -dDotM;
    } else {
        float sqrtOfDiscriminant = Math::squareRoot(discriminant);
        closestT = Math::min(-dDotM + sqrtOfDiscriminant, -dDotM - sqrtOfDiscriminant);
    }
    result.t      = closestT;
    result.point  = ray.origin + ray.direction * result.t;
    result.normal = (result.point - this->position_) / this->radius_;
    result.object = this;
    return true;
}

constexpr Vec3 Sphere::center() const {
    return center_;
}
constexpr float Sphere::radius() const {
    return radius_;
}

// recall, a point lies within or along a sphere if..
// (Px - Cx)^2 + (Py - Cy)^2 + (Pz - Cz)^2 <= R^2
constexpr bool Sphere::contains(const Vec3& point) const {
    return Math::magnitudeSquared(point - center_) <= Math::square(radius_);
}

std::string Sphere::description() const {
    std::stringstream ss;
    ss << "Sphere("
         << "position:(" << center()   << "),"
         << "material:"  << material() << ","
         << "radius:"    << radius()
       << ")";
    return ss.str();
}


Triangle::Triangle(const Vec3& vert0, const Vec3& vert1, const Vec3& vert2, const Material& material) {
    // note that base class members can only be initialized in the body and not our typical initializer list
    assert(isValidTriangle(vert0, vert1, vert2));
    this->vert0_ = vert0;
    this->vert1_ = vert1;
    this->vert2_ = vert2;
    this->edge0_ = vert1 - vert0;
    this->edge1_ = vert2 - vert1;
    this->edge2_ = vert0 - vert2;
    this->planeNormal_ = Math::normalize(Math::cross(edge0_, edge1_));
    this->center_ = computeCentroid();
    this->material_ = material;
    this->position_ = this->center_;
}

// given ray intersects triangle IFF given ray [X(t) = P + tD] intersects the plane [P.n = k] in a way such that
// our intersection point is always to the LEFT side of EVERY edge
// (aka our plane intersection point @[t = (k � P.n)/(D.n)] lies in between our triangle vertices)
bool Triangle::intersect(const Ray& ray, Intersection& result) const {
    const float k = Math::dot(vert0_, planeNormal_);
    const float t = (k - Math::dot(ray.origin, planeNormal_)) / Math::dot(ray.direction, planeNormal_);
    const Vec3 pointIntersectingPlane = ray.origin + ray.direction * t;
    if (contains(pointIntersectingPlane)) {
        result.t      = t;
        result.point  = pointIntersectingPlane;
        result.normal = planeNormal_;
        result.object = this;
        return true;
    }
    return false;
}

constexpr Vec3 Triangle::vert0() const {
    return vert0_;
}
constexpr Vec3 Triangle::vert1() const {
    return vert1_;
}
constexpr Vec3 Triangle::vert2() const {
    return vert2_;
}
constexpr Vec3 Triangle::planeNormal() const {
    return planeNormal_;
}
constexpr Vec3 Triangle::center() const {
    return center_;
}

// recall, a point lies in a triangle if..
// (e0 x(R � P0)).n > 0 & (e1 x(R � P1)).n > 0 & (e2 x(R � P2)).n > 0
// aka R is always to the LEFT side of EVERY edge
constexpr bool Triangle::contains(const Vec3& point) const {
    return (Math::dot(planeNormal_, Math::cross(edge0_, (point - vert0_))) > 0.00f) &&
           (Math::dot(planeNormal_, Math::cross(edge1_, (point - vert1_))) > 0.00f) &&
           (Math::dot(planeNormal_, Math::cross(edge2_, (point - vert2_))) > 0.00f);
}

constexpr Vec3 Triangle::computeCentroid() const {
    return Vec3((vert0_.x + vert1_.x + vert2_.x) / 3.00f,
                (vert0_.y + vert1_.y + vert2_.y) / 3.00f,
                (vert0_.z + vert1_.z + vert2_.z) / 3.00f);
}

// recall that a triangle is formed if each side has a length smaller than the the sum of the other two
bool Triangle::isValidTriangle(const Vec3& vert0, const Vec3& vert1, const Vec3& vert2) {
    float a = Math::distance(vert0, vert1);
    float b = Math::distance(vert1, vert2);
    float c = Math::distance(vert2, vert0);
    return (a + b > c) && (a + c > b) && (b + c > a);
}

std::string Triangle::description() const {
    std::stringstream ss;
    ss << "Triangle("
         << "position:("     << center()      << "),"
         << "plane-normal:(" << planeNormal() << "),"
         << "material:"      << material()    << ","
         << "verts:[v0:(" << vert0() << "),v1:(" << vert1() << "),v2:(" << vert2() << ")]"
       << ")";
    return ss.str();
}
