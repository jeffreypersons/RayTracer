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
    os << "Ray("
         << "origin:("    << ray.origin    << "),"
         << "direction:(" << ray.direction << ")"
       << ")";
    return os;
}
