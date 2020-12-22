#pragma once
#include "Math.hpp"
#include "Colors.hpp"
#include "Rays.hpp"


// virtual base class for ANY renderable (via ray-tracing) object in a scene
class ISceneObject {
private:
    Vec3 DEFAULT_POSITION{ 0, 0, 0 };
    Color DEFAULT_COLOR{ 0.00f, 0.00f, 0.00f };

protected:
    const float EPISILON = 0.000001f;
    Vec3 centroid = DEFAULT_POSITION;
    Material material{ DEFAULT_COLOR, DEFAULT_COLOR, DEFAULT_COLOR, 0.50f, 0.50f };

public:
    ~ISceneObject() {}
    virtual bool intersect(const Ray& ray, RayHitInfo& result) const = 0;
    const Vec3&     getCentroid() const { return centroid; }
    const Material& getMaterial() const { return material; }
};


class Sphere : public ISceneObject {
private:
    float radius;

public:
    float getRadius() const { return radius; }

    Sphere(const Vec3& center, float radius, const Material& material) {
        this->centroid = center;
        this->material = material;
        this->radius   = radius;
    }

    // given ray [X(t) = P + t] intersects sphere [| X – C | = r] IFF there exists
    // at least one real number t [t = - D.M +/- sqrt((D.M)^2 – ( |M|^2 – r^2 ))]
    // (that is, if above discriminant >= 0, then there exists a point along ray that also lies along sphere's surface)
    virtual bool intersect(const Ray& ray, RayHitInfo& result) const override {
        Vec3 M = ray.origin - this->centroid;
        float dDotM = Math::dot(ray.direction, M);
        float discriminant = (dDotM * dDotM) - (Math::magnitudeSquared(M) - radius * radius);
        if (discriminant < 0.00f) {
            return false;
        }

        float closestT;
        // we have an intersection, calculate our t, and plug it in to find our normal/intersect-point
        if (Math::isApproximately(discriminant, 0.00f)) {
            closestT = - dDotM;
        } else {
            float sqrtOfDiscriminant = Math::squareRoot(discriminant);
            closestT = Math::min(- dDotM + sqrtOfDiscriminant, - dDotM - sqrtOfDiscriminant);
        }
        result.t      = closestT;
        result.point  = ray.origin + ray.direction * result.t;
        result.normal = (result.point - this->centroid) / this->radius;
        return true;
    }
};
inline std::ostream& operator<<(std::ostream& os, const Sphere& sphere) {
    os << std::fixed << std::setprecision(2)
       << "Sphere("
         << "centroid:" << sphere.getCentroid() << ", "
         << "material:" << sphere.getMaterial() << ", "
         << "radius:"   << sphere.getRadius()
       << ")";
    return os;
}

class Triangle : public ISceneObject {
private:
    Vec3 vert0, vert1, vert2;
    Vec3 edge0, edge1, edge2;
    Vec3 planeNormal;

    Vec3 computeCentroid() const {
        return Vec3((vert0.x + vert1.x + vert2.x) / 3.00f,
                    (vert0.y + vert1.y + vert2.y) / 3.00f,
                    (vert0.z + vert1.z + vert2.z) / 3.00f);
    }
    // recall, a point lies in a triangle if..
    // (e0 x(R – P0)).n > 0 & (e1 x(R – P1)).n > 0 & (e2 x(R – P2)).n > 0
    // aka R is always to the LEFT side of EVERY edge
    bool isPointInTriangle(const Vec3& point) const {
        return (Math::dot(planeNormal, Math::cross(edge0, (point - vert0))) > 0.00f) &&
               (Math::dot(planeNormal, Math::cross(edge1, (point - vert1))) > 0.00f) &&
               (Math::dot(planeNormal, Math::cross(edge2, (point - vert2))) > 0.00f);
    }
public:
    const Vec3& getVert0()       const { return vert0;       }
    const Vec3& getVert1()       const { return vert1;       }
    const Vec3& getVert2()       const { return vert2;       }
    const Vec3& getPlaneNormal() const { return planeNormal; }

    Triangle(const Vec3& vert0, const Vec3& vert1, const Vec3& vert2, const Material& material) {
        this->centroid = computeCentroid();
        this->material = material;
        this->vert0 = vert0;
        this->vert1 = vert1;
        this->vert2 = vert2;
        this->edge0 = vert1 - vert0;
        this->edge1 = vert2 - vert1;
        this->edge2 = vert0 - vert2;
        this->planeNormal = Math::normalize(Math::cross(edge0, edge1));
    }

    // given ray intersects triangle IFF given ray [X(t) = P + tD] intersects the plane [P.n = k] in a way such that
    // our intersection point is always to the LEFT side of EVERY edge
    // (aka our plane intersection point @[t = (k – P.n)/(D.n)] lies in between our triangle vertices)
    virtual bool intersect(const Ray& ray, RayHitInfo& result) const override {
        float k = Math::dot(vert0, planeNormal);
        float t = (k - Math::dot(ray.origin, planeNormal)) / Math::dot(ray.direction, planeNormal);
        Vec3 pointIntersectingPlane = ray.origin + ray.direction * t;
        if (isPointInTriangle(pointIntersectingPlane)) {
            result.t      = t;
            result.point  = pointIntersectingPlane;
            result.normal = planeNormal;
            return true;
        }
        return false;
    }
};
inline std::ostream& operator<<(std::ostream& os, const Triangle& triangle) {
    os << std::fixed << std::setprecision(2)
       << "Triangle("
         << "centroid:"  << triangle.getCentroid() << ", "
         << "material:"  << triangle.getMaterial() << ", "
         << "verts:[v0:" << triangle.getVert0() << ",v1:" << triangle.getVert1() << ",v2:" << triangle.getVert2() << "]"
       << ")";
    return os;
}