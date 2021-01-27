#pragma once
#include "Math.hpp"
#include "Material.hpp"
#include "Rays.hpp"


// virtual base class for ANY renderable (via ray-tracing) object in a scene
class ISceneObject {
protected:
    Vec3 centroid{};
    Material material{};

public:
    ~ISceneObject() {}

    const Vec3& getCentroid() const { return centroid; }
    const Material& getMaterial() const { return material; }

    virtual std::string getDescription() const = 0;
    virtual bool intersect(const Ray& ray, RayHitInfo& result) const = 0;
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
    virtual bool intersect(const Ray&, RayHitInfo&) const override;
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
    virtual bool intersect(const Ray& ray, RayHitInfo& result) const override;
    const Vec3& getVert0()       const { return vert0;       }
    const Vec3& getVert1()       const { return vert1;       }
    const Vec3& getVert2()       const { return vert2;       }
    const Vec3& getPlaneNormal() const { return planeNormal; }
};
