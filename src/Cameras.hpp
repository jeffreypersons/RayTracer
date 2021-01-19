#pragma once
#include "Math.hpp"
#include "Color.hpp"
#include "Rays.hpp"


class RenderCam {
private:
    Vec3 position;
    Vec3 aimDir;
    Vec3 rightDir;
    Vec3 upDir;
    float viewportHeight;
    float viewportWidth;
    float viewportDistance;
    
public:
    RenderCam() :
        position(0, 0, 0),
        aimDir(0, 0, -1),
        rightDir(1, 0, 0),
        upDir(0, 1, 0),
        viewportWidth(1.00f),
        viewportHeight(1.00f),
        viewportDistance(1.00f) {}

    // set orientation manually
    // caution: note that no checking for orthogonality or normalization is done
    void setOrientation(const Vec3& aimDir, const Vec3& rightDir, const Vec3& upDir) {
        this->aimDir   = aimDir;
        this->rightDir = rightDir;
        this->upDir    = upDir;
    }
    void setPosition(const Vec3& position) {
        this->position = position;
    }
    void setViewportSize(float viewportWidth, float viewportHeight) {
        this->viewportWidth  = viewportWidth;
        this->viewportHeight = viewportHeight;
    }
    void setViewDistance(float viewportDistance) {
        this->viewportDistance = viewportDistance;
    }

    void lookAt(const Vec3& target, const Vec3& relativeUp) {
        Vec3 givenUp      = relativeUp;
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
    
    const Vec3& getUpDir()    const { return upDir;    }
    const Vec3& getRightDir() const { return rightDir; }
    const Vec3& getAimDir()   const { return aimDir;   }
    const Vec3& getPosition() const { return position; }
    
    Vec3 viewportToWorld(float u, float v) const {
        float centeredU = u - 0.50f;
        float centeredV = v - 0.50f;
        Vec3 viewportCenter   = position + (viewportDistance * aimDir);
        Vec3 offsetInRightDir = centeredU * viewportWidth  * rightDir;
        Vec3 offsetInUpDir    = centeredV * viewportHeight * upDir;
        return viewportCenter + offsetInRightDir + offsetInUpDir;
    }
    // get a ray from current cam position to (u, v) position on our viewplane
    // note that uv values outside of [0, 1] are outside of camera view
    Ray getRay(float u, float v) const {
        return Ray(position, Math::direction(position, viewportToWorld(u, v)));
    }
};
