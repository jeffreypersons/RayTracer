#pragma once
#include "Math.hpp"
#include "Colors.hpp"
#include "Rays.hpp"


// viewplane zDistance 'into' screen, facing directly back towards the screen
// note: no clipping or culling is done
class ViewPlane {
private:
    float width;
    float height;
    float zDepth;
    Vec3 min;
    Vec3 max;
    Vec3 normal;

public:
    ViewPlane(float width, float height, float zDepth) :
        width(width), height(height), zDepth(zDepth),
        min(width * -0.50f, height * -0.50f, zDepth),
        max(width *  0.50f, height *  0.50f, zDepth),
        normal(0, 0, 1) {}

    float aspect()     const { return width / height;             }
    float depth()      const { return zDepth;                     }
    Vec3 center()      const { return Vec3(min.x, max.y, zDepth); }
    Vec3 topLeft()     const { return Vec3(min.x, max.y, zDepth); }
    Vec3 topRight()    const { return max;                        }
    Vec3 bottomRight() const { return Vec3(max.x, min.y, zDepth); }
    Vec3 bottomLeft()  const { return min;                        }
    // assumes viewport-point(u,v) in range[0, 1]
    Vec3 toWorld(float u, float v) const {
        return Vec3(min.x + (u * width), min.y + (v * height), zDepth);
    }
};

// 3d camera fixed in middle with a viewplane z distance from camera
// up and look vectors are fixed to (0, 1, 0) and (0, 0, -1) respectively,
// from position (0, 0, zDepth)
class RenderCam {
private:
    Vec3 upDir;
    Vec3 aimDir;
    Vec3 position;
    ViewPlane viewPlane;

public:
    RenderCam(const ViewPlane& viewPlane) :
        upDir(0, 1, 0),
        aimDir(0, 0, -1),
        position(0, 0, 0),
        viewPlane(viewPlane) {}

    // get a ray from current cam position to (u, v) position on our viewplane
    Ray getRay(float u, float v) const {
        Vec3 pointOnPlane = viewPlane.toWorld(u, v);
        return Ray(position, Math::direction(position, pointOnPlane));
    }
    const Vec3& getUpDir()          const { return upDir;     }
    const Vec3& getAimDir()         const { return aimDir;    }
    const Vec3& getPosition()       const { return position;  }
    const ViewPlane& getViewPlane() const { return viewPlane; }
};
