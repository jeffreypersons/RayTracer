#pragma once
#include "Math.hpp"
#include "Color.hpp"
#include "Rays.hpp"


// z-axis-aligned perspective 3d camera (rectangular-pinhole cam without lens)
// note: here, image plane is equivalent to the near plane
// note: fieldOfView is taken as horizontal fov, with the aspect ratio determining the verticalFov
class RenderCam {
private:
    Vec3 position;
    Vec3 rightDir;
    Vec3 forwardDir;
    Vec3 upDir;

    float nearClip;
    float farClip;
    float aspectRatio;
    Vec2 fieldOfView;
    Vec2 viewportSize;

    static constexpr float MIN_FIELD_OF_VIEW =   0.00f;
    static constexpr float MAX_FIELD_OF_VIEW = 180.00f;

    static constexpr Vec3 DEFAULT_POSITION    {  0,  0,  0  };
    static constexpr Vec3 DEFAULT_FORWARD_DIR {  0,  0, -1  };
    static constexpr Vec3 DEFAULT_RIGHT_DIR   {  1,  0,  0  };
    static constexpr Vec3 DEFAULT_UP_DIR      {  0,  1,  0  };

    static constexpr float DEFAULT_ASPECT_RATIO  = 4.0f / 3.0f;
    static constexpr float DEFAULT_FIELD_OF_VIEW =      60.00f;
    static constexpr float DEFAULT_DISTANCE      =    1000.00f;
    static constexpr float DEFAULT_NEAR_CLIP     =       0.25f;
    static constexpr float DEFAULT_FAR_CLIP      =    1000.00f;
    
    // find field of view by solving for the fov in the equation `tan(fov * 0.5) = (0.5 * len) / dist
    static Vec2 computeFieldOfView(Vec2 viewportSize, float distanceToPlane) {
        float fieldOfViewHorizontal = 2.00f * Math::atan(viewportSize.x / (2.00f * distanceToPlane));
        float fieldOfViewVertical   = 2.00f * Math::atan(viewportSize.y / (2.00f * distanceToPlane));
        return Vec2(fieldOfViewHorizontal, fieldOfViewVertical);
    }
    // find viewport size by solving for the triangular len (width/height) in the equation `tan(fov * 0.5) = (0.5 * len) / dist`
    static Vec2 computeViewplaneSize(Vec2 fieldOfView, float distanceToPlane) {
        float viewportWidth  = 2.00f * distanceToPlane * Math::tan(0.50f * fieldOfView.x);
        float viewportHeight = 2.00f * distanceToPlane * Math::tan(0.50f * fieldOfView.y);
        return Vec2(viewportWidth, viewportHeight);
    }

public:
    RenderCam() {
        setPosition(DEFAULT_POSITION);
        setOrientation(DEFAULT_RIGHT_DIR, DEFAULT_FORWARD_DIR, DEFAULT_UP_DIR);
        setFieldOfView(DEFAULT_FIELD_OF_VIEW);
        setAspectRatio(DEFAULT_ASPECT_RATIO);
        setFarClip(DEFAULT_FAR_CLIP);
    }
    
    const Vec3& getPosition()        const { return position;      }
    const Vec3& getUpDir()           const { return upDir;         }
    const Vec3& getRightDir()        const { return rightDir;      }
    const Vec3& getForwardDir()      const { return forwardDir;    }
    float getHorizontalFieldOfView() const { return fieldOfView.x; }
    float getVerticalFieldOfView()   const { return fieldOfView.y; }
    float getAspectRatio()           const { return aspectRatio;   }
    float getNearClip()              const { return nearClip;      }
    float getFarClip()               const { return farClip;       }
    const Vec2& getViewportSize()    const { return viewportSize;  }

    Vec3 viewportToWorld(float u, float v) const {
        float centeredU = u - 0.50f;
        float centeredV = v - 0.50f;
        Vec3 viewportCenter   = position + (nearClip * forwardDir);
        Vec3 offsetInRightDir = centeredU * viewportSize.x * rightDir;
        Vec3 offsetInUpDir    = centeredV * viewportSize.y * upDir;
        return viewportCenter + offsetInRightDir + offsetInUpDir;
    }
    // get a ray from current cam position to (u, v) position on our viewplane
    // note that uv values outside of [0, 1] are outside of camera view
    Ray getRay(float u, float v) const {
        return Ray(position, Math::direction(position, viewportToWorld(u, v)));
    }

    void setPosition(const Vec3& position) {
        this->position = position;
    }
    void setOrientation(const Vec3& rightDir, const Vec3& forwardDir, const Vec3& upDir) {
        if (!Math::isNormalized(forwardDir) || !Math::isNormalized(rightDir) || !Math::isNormalized(upDir)) {
            throw std::invalid_argument("all given orientation vectors must be normalized");
        }
        if (!Math::isOrthogonal(rightDir, forwardDir) || !Math::isOrthogonal(forwardDir, upDir)) {
            throw std::invalid_argument("all given orientation vectors must be orthogonal to one another");
        }
        if (Math::isParallelDirection(upDir, forwardDir)) {
            throw std::invalid_argument("cannot have up parallel to aim direction");
        }

        this->forwardDir = forwardDir;
        this->rightDir   = rightDir;
        this->upDir      = upDir;
    }

    // sets field of view, aspect ratio, and clipping planes
    // compute field of view by solving for the fov in the equation `tan(fov * 0.5) = (0.5 * width) / dist`
    void overrideViewportSize(Vec2 viewportSize, float distanceToViewport) {
        if (viewportSize.x <= 0 || viewportSize.y <= 0) {
            throw std::invalid_argument("viewport size dimensions must each be greater than 0");
        }

        this->nearClip     = distanceToViewport;
        this->farClip      = 10.00f * distanceToViewport;
        this->aspectRatio  = viewportSize.x / viewportSize.y;
        this->fieldOfView  = computeFieldOfView(viewportSize, distanceToViewport);
        this->viewportSize = viewportSize;
    }

    void setFieldOfView(float horizontalDegrees) {
        if (horizontalDegrees <= MIN_FIELD_OF_VIEW || horizontalDegrees >= MAX_FIELD_OF_VIEW) {
            throw std::invalid_argument("field-of-view must be in range (0, 180)");
        }

        this->fieldOfView  = Vec2(horizontalDegrees, horizontalDegrees / aspectRatio);
        this->viewportSize = computeViewplaneSize(fieldOfView, nearClip);
    }
    void setAspectRatio(float aspectRatio) {
        if (aspectRatio <= 0) {
            throw std::invalid_argument("aspect-ratio must be greater than 0");
        }

        this->aspectRatio  = aspectRatio;
        this->fieldOfView  = Vec2(fieldOfView.x, fieldOfView.x / aspectRatio);
        this->viewportSize = computeViewplaneSize(fieldOfView, nearClip);
    }
    
    void setNearClip(float nearClip) {
        if (nearClip <= 0 || nearClip >= farClip) {
            throw std::invalid_argument("near-clip must be in range of (0, farClip)");
        }
        this->nearClip = nearClip;
        this->fieldOfView = computeFieldOfView(viewportSize, nearClip);
        this->fieldOfView = Vec2(fieldOfView.x, fieldOfView.x / aspectRatio);
        this->viewportSize = computeViewplaneSize(fieldOfView, nearClip);
    }
    void setFarClip(float farClip) {
        if (farClip < 0) {
            throw std::invalid_argument("far-clip must be greater than 0");
        }
        this->farClip = farClip;
    }

    void lookAt(const Vec3& target, const Vec3& relativeUp) {
        if (Math::isNormalized(relativeUp)) {
            throw std::invalid_argument("relative up must be a normalized direction");
        }

        Vec3 givenUp = relativeUp;
        Vec3 givenForward = Math::direction(position, target);
        if (Math::isParallelDirection(givenUp, givenForward)) {
            throw std::invalid_argument("cannot look in a direction parallel to given up direction");
        }

        forwardDir = givenForward;
        rightDir   = Math::normalize(Math::cross(forwardDir, givenUp));
        upDir      = Math::cross(rightDir, forwardDir);
    }
};
inline std::ostream& operator<<(std::ostream& os, const RenderCam& renderCam) {
    os << std::fixed << std::setprecision(2)
       << "RenderCam("
         << "Position:" << renderCam.getPosition() << ", "
         << "Orientation{"
           << "right-axis:("   << renderCam.getRightDir()   << "),"
           << "forward-axis:(" << renderCam.getForwardDir() << "),"
           << "upward-axis:("  << renderCam.getUpDir()      << ")}, "
         << "ImagePlane{"
           << "aspect-ratio:"    << renderCam.getAspectRatio()           << ","
           << "field-of-view-x:" << renderCam.getHorizontalFieldOfView() << ","
           << "field-of-view-y:" << renderCam.getVerticalFieldOfView()   << ","
           << "viewport-width:"  << renderCam.getViewportSize().x        << ","
           << "viewport-height:" << renderCam.getViewportSize().y        << "}, "
         << "Clipping{"
           << "near-plane-z:" << renderCam.getNearClip() << ","
           << "far-plane-z:"  << renderCam.getFarClip()  << "}"
         << ")";
    return os;
}
