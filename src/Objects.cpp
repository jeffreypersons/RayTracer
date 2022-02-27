#include "Objects.hpp"
#include "Math.hpp"
#include "Ray.hpp"
#include <assert.h>


Sphere::Sphere(const Vec3& center, float radius, const Material& material)
    : center_(center),
      radius_(radius) {
    assert(radius > 0.00f);
    this->position_ = center;
    this->material_ = material;
}

// Ray: P_r = P_0 + d * t
// Sphere: (P_s - P_c).(P_s - P_c) - r^2 = 0
// Intersection: P_r == P_s
//
// (P_0 + d * t - P_c).(P_0 + d * t - P_c) - r^2 = 0
// L := P_0 - P_c
// (d * t + L).(d * t + L) - r^2 = 0
// d.d * t^2 + 2 * d.L * t + L.L - r^2 = 0
//
// quadratic coefficients:
// a = d.d
// b = 2 * d.L
// c = L.L - r^2
//
// t_intersect = -b +/- sqrt(b^2 - 4*a*c)
//
bool Sphere::intersect(const Ray& ray, Intersection& result) const {
    const Vec3 L = ray.origin - this->position_;
    // const float a = Math::dot(ray.direction, ray.direction); a == 1.0
    const float b = 2.0f * Math::dot(ray.direction, L);
    const float c = Math::dot(L, L) - Math::square(this->radius_);
    const float discriminant = Math::square(b) - 4.0f * c;

    if (discriminant < 0.00f) {
        return false;
    }

    // we have an intersection, calculate our t, and plug it in to find our normal/intersect-point
    float t1, t2;
    if (Math::isApproximately(discriminant, 0.00f)) {
        t1 = t2 = -b / 2.0f;
    } else {
        float sqrtOfDiscriminant = Math::squareRoot(discriminant);
        t1 = (-b + sqrtOfDiscriminant) / 2.0f;
        t2 = (-b - sqrtOfDiscriminant) / 2.0f;
    }

    if (t1 < 0.00f && t2 < 0.00f) {
        return false;
    }

    if (t1 < 0.00f) {
        result.t = t2;
    }
    else if (t2 < 0.00f) {
        result.t = t1;
    }
    else {
        result.t = std::min(t1, t2);
    }

    result.point  = ray.origin + ray.direction * result.t;
    result.normal = Math::direction(this->position_, result.point);
    result.object = this;

    return true;
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


// recall, a point lies within or along a sphere if..
// (Px - Cx)^2 + (Py - Cy)^2 + (Pz - Cz)^2 <= R^2
bool Sphere::contains(const Vec3& point) const {
    return Math::magnitudeSquared(point - center_) <= Math::square(radius_);
}


Vec3 Sphere::center() const {
    return center_;
}

float Sphere::radius() const {
    return radius_;
}




Triangle::Triangle(const Vec3& vert0, const Vec3& vert1, const Vec3& vert2, const Material& material)
    : vert0_      (vert0),
      vert1_      (vert0),
      vert2_      (vert0),
      edge0_      (vert1 - vert0),
      edge1_      (vert2 - vert1),
      edge2_      (vert0 - vert2),
      center_     (computeCentroid()),
      planeNormal_(Math::normalize(Math::cross(edge0_, edge1_))) {
    assert(isValidTriangle(vert0, vert1, vert2));
    this->position_ = center_;
    this->material_ = material;
}

// given ray intersects triangle IFF given ray [X(t) = P + tD] intersects the plane [P.n = k] in a way such that
// our intersection point is always to the LEFT side of EVERY edge
// (aka our plane intersection point @[t = (k – P.n)/(D.n)] lies in between our triangle vertices)
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


// recall, a point lies in a triangle if..
// (e0 x(R – P0)).n > 0 & (e1 x(R – P1)).n > 0 & (e2 x(R – P2)).n > 0
// aka R is always to the LEFT side of EVERY edge
bool Triangle::contains(const Vec3& point) const {
    return (Math::dot(planeNormal_, Math::cross(edge0_, (point - vert0_))) > 0.00f) &&
           (Math::dot(planeNormal_, Math::cross(edge1_, (point - vert1_))) > 0.00f) &&
           (Math::dot(planeNormal_, Math::cross(edge2_, (point - vert2_))) > 0.00f);
}


Vec3 Triangle::vert0() const {
    return vert0_;
}

Vec3 Triangle::vert1() const {
    return vert1_;
}

Vec3 Triangle::vert2() const {
    return vert2_;
}

Vec3 Triangle::center() const {
    return center_;
}

Vec3 Triangle::planeNormal() const {
    return planeNormal_;
}


Vec3 Triangle::computeCentroid() const {
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
