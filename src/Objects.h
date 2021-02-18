#pragma once
#include "Math.hpp"
#include "Material.hpp"
#include "Ray.hpp"


// virtual base class for ANY renderable (via ray-tracing) object in a scene
class IObject {
protected:
    Vec3 centroid{};
    Material material{};

public:
    ~IObject() {}

    virtual bool intersect(const Ray& ray, Intersection& result) const = 0;
    virtual std::string getDescription() const = 0;

    Vec3     getCentroid() const { return centroid; }
    Material getMaterial() const { return material; }
};
inline std::ostream& operator<<(std::ostream& os, const IObject& object) {
    os << object.getDescription();
    return os;
}


class Sphere : public IObject {
private:
    float radius;

public:
    Sphere(const Vec3& center, float radius, const Material& material);
    virtual bool intersect(const Ray& ray, Intersection& result) const override;
    virtual std::string getDescription() const override;

    float getRadius() const;
};


class Triangle : public IObject {
private:
    Vec3 vert0;
    Vec3 vert1;
    Vec3 vert2;
    Vec3 edge0;
    Vec3 edge1;
    Vec3 edge2;
    Vec3 planeNormal;
    
public:
    Triangle(const Vec3& vert0, const Vec3& vert1, const Vec3& vert2, const Material& material);
    virtual bool intersect(const Ray& ray, Intersection& result) const override;
    virtual std::string getDescription() const override;

    Vec3 getVert0()       const;
    Vec3 getVert1()       const;
    Vec3 getVert2()       const;
    Vec3 getPlaneNormal() const;

private:
    Vec3 computeCentroid() const;
    bool isPointInTriangle(const Vec3& point) const;
};
