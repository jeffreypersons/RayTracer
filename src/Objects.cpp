#include "Objects.h"
#include "Math.hpp"
#include "Ray.hpp"


// assumes radius greater than zero
Sphere::Sphere(const Vec3& center, float radius, const Material& material) {
    // note that base class members can only be initialized in the body and not our typical initializer list
    this->centroid = center;
    this->material = material;
    this->radius   = radius;
}

// given ray [X(t) = P + t] intersects sphere [| X – C | = r] IFF there exists
// at least one real number t [t = - D.M +/- sqrt((D.M)^2 – ( |M|^2 – r^2 ))]
// (that is, if above discriminant >= 0, then there exists a point along ray that also lies along sphere's surface)
bool Sphere::intersect(const Ray& ray, Intersection& result) const {
    Vec3 M = ray.origin - this->centroid;
    float dDotM = Math::dot(ray.direction, M);
    float discriminant = (dDotM * dDotM) - (Math::magnitudeSquared(M) - radius * radius);
    if (discriminant < 0.00f) {
        return false;
    }

    // we have an intersection, calculate our t, and plug it in to find our normal/intersect-point
    float closestT;
    if (Math::isApproximately(discriminant, 0.00f)) {
        closestT = - dDotM;
    } else {
        float sqrtOfDiscriminant = Math::squareRoot(discriminant);
        closestT = Math::min(-dDotM + sqrtOfDiscriminant, -dDotM - sqrtOfDiscriminant);
    }
    result.t      = closestT;
    result.point  = ray.origin + ray.direction * result.t;
    result.normal = (result.point - this->centroid) / this->radius;
    result.object = this;
    return true;
}

float Sphere::getRadius() const {
    return radius;
}

std::string Sphere::getDescription() const {
    std::stringstream ss;
    ss << "Sphere("
         << "centroid:(" << getCentroid() << "),"
         << "material:"  << getMaterial() << ","
         << "radius:"    << getRadius()
       << ")";
    return ss.str();
}


Triangle::Triangle(const Vec3& vert0, const Vec3& vert1, const Vec3& vert2, const Material& material) {
    // note that base class members can only be initialized in the body and not our typical initializer list
    this->material = material;
    this->vert0 = vert0;
    this->vert1 = vert1;
    this->vert2 = vert2;
    this->edge0 = vert1 - vert0;
    this->edge1 = vert2 - vert1;
    this->edge2 = vert0 - vert2;
    this->centroid    = computeCentroid();
    this->planeNormal = Math::normalize(Math::cross(edge0, edge1));
}

// given ray intersects triangle IFF given ray [X(t) = P + tD] intersects the plane [P.n = k] in a way such that
// our intersection point is always to the LEFT side of EVERY edge
// (aka our plane intersection point @[t = (k – P.n)/(D.n)] lies in between our triangle vertices)
bool Triangle::intersect(const Ray& ray, Intersection& result) const {
    float k = Math::dot(vert0, planeNormal);
    float t = (k - Math::dot(ray.origin, planeNormal)) / Math::dot(ray.direction, planeNormal);
    Vec3 pointIntersectingPlane = ray.origin + ray.direction * t;
    if (isPointInTriangle(pointIntersectingPlane)) {
        result.t      = t;
        result.point  = pointIntersectingPlane;
        result.normal = planeNormal;
        result.object = this;
        return true;
    }
    return false;
}

Vec3 Triangle::computeCentroid() const {
    return Vec3((vert0.x + vert1.x + vert2.x) / 3.00f,
                (vert0.y + vert1.y + vert2.y) / 3.00f,
                (vert0.z + vert1.z + vert2.z) / 3.00f);
}
// recall, a point lies in a triangle if..
// (e0 x(R – P0)).n > 0 & (e1 x(R – P1)).n > 0 & (e2 x(R – P2)).n > 0
// aka R is always to the LEFT side of EVERY edge
bool Triangle::isPointInTriangle(const Vec3& point) const {
    return (Math::dot(planeNormal, Math::cross(edge0, (point - vert0))) > 0.00f) &&
           (Math::dot(planeNormal, Math::cross(edge1, (point - vert1))) > 0.00f) &&
           (Math::dot(planeNormal, Math::cross(edge2, (point - vert2))) > 0.00f);
}

Vec3 Triangle::getVert0() const {
    return vert0;
}
Vec3 Triangle::getVert1() const {
    return vert1;
}
Vec3 Triangle::getVert2() const {
    return vert2;
}
Vec3 Triangle::getPlaneNormal() const {
    return planeNormal;
}

std::string Triangle::getDescription() const {
    std::stringstream ss;
    ss << "Triangle("
         << "centroid:("        << getCentroid()    << "),"
         << "plane-normal:(v0:" << getPlaneNormal() << "),"
         << "material:"         << getMaterial()    << ","
         << "verts:[v0:(" << getVert0() << "),v1:(" << getVert1() << "),v2:(" << getVert2() << ")]"
       << ")";
    return ss.str();
}
