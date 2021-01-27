#pragma once
#include "Math.hpp"


// assumes direction is normalized
struct Ray {
    Vec3 origin;
    Vec3 direction;
    Ray() = default;
    constexpr Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}
};
inline std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    os << std::fixed << std::setprecision(2)
       << "Ray("
         << "origin:("    << ray.origin     << "),"
         << "direction:(" << ray.direction  << ")"
       << ")";
    return os;
}

struct RayHitInfo {
    Vec3 point;
    Vec3 normal;
    float t;
    RayHitInfo() = default;
    constexpr RayHitInfo(const Vec3& point, const Vec3& normal, float t) :
        point(point),
        normal(normal),
        t(t) {}
};
inline std::ostream& operator<<(std::ostream& os, const RayHitInfo& intersectInfo) {
    os << std::fixed << std::setprecision(2)
       << "IntersectInfo("
         << "point:("  << intersectInfo.point  << "),"
         << "normal:(" << intersectInfo.normal << "),"
         << "t:" << intersectInfo.t
       << ")";
    return os;
}
