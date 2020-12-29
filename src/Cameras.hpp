#pragma once
#include "Math.hpp"
#include "Color.hpp"
#include "Rays.hpp"


// viewplane zDistance 'into' screen, facing directly back towards the screen
class Viewport {
private:
    float width;
    float height;
    float zDepth;
    Vec3 min;
    Vec3 max;
    Vec3 normal;

public:
    Viewport(float width, float height, float zDepth) :
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

class RenderCam {
private:
    Vec3 upDir;
    Vec3 aimDir;
    Vec3 rightDir;
    Vec3 position;
    Viewport viewport;

public:
    RenderCam(const Viewport& viewport) :
        upDir(0, 1, 0),
        aimDir(0, 0, -1),
        rightDir(1, 0, 0),
        position(0, 0, 0),
        viewport(viewport) {}

    void setPosition(const Vec3& position) {
        this->position = position;
    }

    void lookAt(const Vec3& target, const Vec3& worldUp) {
        Vec3 givenUp      = worldUp;
        Vec3 givenForward = Math::direction(position, target);
        if (Math::isParallelDirection(givenUp, givenForward)) {
            throw std::exception("cannot look at given position - "
                                 "inferred look direction cannot be parallel to given up direction");
            return;
        }

        aimDir   = givenForward;
        rightDir = Math::normalize(Math::cross(aimDir, givenUp));
        upDir    = Math::cross(rightDir, aimDir);
    }

    const Vec3& getUpDir()        const { return upDir;    }
    const Vec3& getRightDir()     const { return rightDir; }
    const Vec3& getAimDir()       const { return aimDir;   }
    const Vec3& getPosition()     const { return position; }
    const Viewport& getViewport() const { return viewport; }
    
    // get a ray from current cam position to (u, v) position on our viewplane
    Ray getRay(float u, float v) const {
        Vec3 pointOnPlane = (u * rightDir) + (v * upDir) + position + aimDir;
        return Ray(position, Math::direction(position, pointOnPlane));
    }
};
