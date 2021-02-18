#pragma once
#include "Math.hpp"
#include "Material.hpp"
#include "Ray.hpp"


class ISceneObject;

struct Intersection {
    Vec3 point;
    Vec3 normal;
    float t;
    const ISceneObject* object;

    constexpr Intersection() : Intersection(Vec3(), Vec3(), -1.00f, nullptr) {}
    constexpr Intersection(const Vec3& point, const Vec3& normal, float t, ISceneObject* object) :
        point(point),
        normal(normal),
        t(t),
        object(object)
    {}
};
inline std::ostream& operator<<(std::ostream& os, const Intersection& intersectInfo) {
    os << "Intersection("
         << "point:("  << intersectInfo.point  << "),"
         << "normal:(" << intersectInfo.normal << "),"
         << "t:"       << intersectInfo.t
       << ")";
    return os;
}

// virtual base class for ANY renderable (via ray-tracing) object in a scene
class ISceneObject {
protected:
    Vec3 centroid{};
    Material material{};

public:
    ~ISceneObject() {}

    Vec3 getCentroid()     const { return centroid; }
    Material getMaterial() const { return material; }

    virtual std::string getDescription() const = 0;
    virtual bool intersect(const Ray& ray, Intersection& result) const = 0;
};
inline std::ostream& operator<<(std::ostream& os, const ISceneObject& sceneObject) {
    os << sceneObject.getDescription();
    return os;
}


class Sphere : public ISceneObject {
private:
    float radius;

public:
    Sphere(const Vec3&, float, const Material&);
    virtual std::string getDescription() const override;
    virtual bool intersect(const Ray&, Intersection&) const override;
    float getRadius() const { return radius; }
};


class Triangle : public ISceneObject {
private:
    Vec3 vert0;
    Vec3 vert1;
    Vec3 vert2;
    Vec3 edge0;
    Vec3 edge1;
    Vec3 edge2;
    Vec3 planeNormal;
    
    Vec3 computeCentroid() const;
    bool isPointInTriangle(const Vec3& point) const;
public:
    Triangle(const Vec3&, const Vec3&, const Vec3&, const Material&);
    virtual std::string getDescription() const override;
    virtual bool intersect(const Ray& ray, Intersection& result) const override;
    Vec3 getVert0()       const { return vert0;       }
    Vec3 getVert1()       const { return vert1;       }
    Vec3 getVert2()       const { return vert2;       }
    Vec3 getPlaneNormal() const { return planeNormal; }
};
