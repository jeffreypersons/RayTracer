#pragma once
#include "Math.hpp"
#include "Rays.hpp"


namespace CommonResolutions {
constexpr Vec2 SD_240p  = Vec2( 426,  240);
constexpr Vec2 SD_360p  = Vec2( 640,  360);
constexpr Vec2 SD_480p  = Vec2( 854,  480);
constexpr Vec2 HD_720p  = Vec2(1280,  720);
constexpr Vec2 HD_1080p = Vec2(1920, 1080);
constexpr Vec2 HD_2K    = Vec2(2560, 1440);
constexpr Vec2 HD_4K    = Vec2(3840, 2160);
constexpr Vec2 HD_8K    = Vec2(7680, 4320);
}

// z-axis-aligned perspective 3d camera (rectangular-pinhole cam without lens)
// note: here, image plane is equivalent to the near plane
// note: fieldOfView is taken as horizontal fov, with the aspect ratio determining the verticalFov
class RenderCam {
private:
    Vec3 position;
    Vec3 rightDir;
    Vec3 upDir;
    Vec3 forwardDir;

    float nearClip;
    float farClip;
    float aspectRatio;
    Vec2 fieldOfView;
    Vec2 viewportSize;

    static constexpr float MIN_FIELD_OF_VIEW =   0.00f;
    static constexpr float MAX_FIELD_OF_VIEW = 180.00f;

    static constexpr Vec3 DEFAULT_POSITION    = Vec3::zero();
    static constexpr Vec3 DEFAULT_RIGHT_DIR   = Vec3::right();
    static constexpr Vec3 DEFAULT_UP_DIR      = Vec3::up();
    static constexpr Vec3 DEFAULT_FORWARD_DIR = Vec3::ahead();

    static constexpr float DEFAULT_ASPECT_RATIO  = 4.0f / 3.0f;
    static constexpr float DEFAULT_FIELD_OF_VIEW =      60.00f;
    static constexpr float DEFAULT_DISTANCE      =    1000.00f;
    static constexpr float DEFAULT_NEAR_CLIP     =       0.25f;
    static constexpr float DEFAULT_FAR_CLIP      =    1000.00f;
    
    // find viewport size by solving for the triangular len (width/height) in the equation `tan(fov * 0.5) = (0.5 * len) / dist`
    Vec2 computeSizeFromHorizontalFov(float horizontalFieldOfView, float distanceToPlane) {
        float viewportWidth = 2.00f * distanceToPlane * Math::tan(0.50f * fieldOfView.x);
        float viewportHeight = viewportWidth / aspectRatio;
        return Vec2(viewportWidth, viewportHeight);
    }
    // find field of view by solving for the fov in the equation `tan(fov * 0.5) = (0.5 * len) / dist
    void setupPerspectiveFromSize(const Vec2& viewportSize, float distanceToPlane) {
        this->nearClip      = distanceToPlane;
        this->farClip       = 10.00f * distanceToPlane;
        this->viewportSize  = viewportSize;
        this->aspectRatio   = viewportSize.x / viewportSize.y;
        this->fieldOfView.x = 2.00f * Math::atan(viewportSize.x / (2.00f * distanceToPlane));
        this->fieldOfView.y = 2.00f * Math::atan(viewportSize.y / (2.00f * distanceToPlane));
    }

public:
    RenderCam() {
        setPosition(DEFAULT_POSITION);
        setOrientation(DEFAULT_RIGHT_DIR, DEFAULT_UP_DIR, DEFAULT_FORWARD_DIR);
        setupPerspectiveFromSize(computeSizeFromHorizontalFov(DEFAULT_FIELD_OF_VIEW, DEFAULT_NEAR_CLIP), DEFAULT_NEAR_CLIP);
        setFarClip(DEFAULT_FAR_CLIP);
    }
    
    Vec3 getPosition()               const { return position;      }
    Vec3 getRightDir()               const { return rightDir;      }
    Vec3 getUpDir()                  const { return upDir;         }
    Vec3 getForwardDir()             const { return forwardDir;    }
    float getHorizontalFieldOfView() const { return fieldOfView.x; }
    float getVerticalFieldOfView()   const { return fieldOfView.y; }
    float getAspectRatio()           const { return aspectRatio;   }
    float getNearClip()              const { return nearClip;      }
    float getFarClip()               const { return farClip;       }
    Vec2 getViewportSize()           const { return viewportSize;  }

    Vec3 viewportToWorld(float u, float v) const {
        float centeredU = u - 0.50f;
        float centeredV = v - 0.50f;
        Vec3 viewportCenter   = position + (nearClip * forwardDir);
        Vec3 offsetInRightDir = (centeredU * viewportSize.x * rightDir);
        Vec3 offsetInUpDir    = (centeredV * viewportSize.y * upDir);
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
    void setOrientation(const Vec3& rightDir, const Vec3& upDir, const Vec3& forwardDir) {
        if (!Math::isNormalized(forwardDir) || !Math::isNormalized(rightDir) || !Math::isNormalized(upDir)) {
            throw std::invalid_argument("all given orientation vectors must be normalized");
        }
        if (!Math::isOrthogonal(rightDir, forwardDir) || !Math::isOrthogonal(forwardDir, upDir)) {
            throw std::invalid_argument("all given orientation vectors must be orthogonal to one another");
        }
        this->rightDir   = rightDir;
        this->upDir      = upDir;
        this->forwardDir = forwardDir;
    }

    // overrides field of view, aspect ratio, and clipping planes to match given viewport dimensions
    // compute field of view by solving for the fov in the equation `tan(fov * 0.5) = (0.5 * width) / dist`
    void overrideViewportSize(Vec2 viewportSize, float distanceToViewport) {
        if (viewportSize.x <= 0 || viewportSize.y <= 0) {
            throw std::invalid_argument("viewport size dimensions must each be greater than 0");
        }
        setupPerspectiveFromSize(viewportSize, distanceToViewport);
    }

    void setFieldOfView(float horizontalDegrees) {
        if (horizontalDegrees <= MIN_FIELD_OF_VIEW || horizontalDegrees >= MAX_FIELD_OF_VIEW) {
            throw std::invalid_argument("field-of-view must be in range (0, 180)");
        }
        Vec2 adjustedSize = computeSizeFromHorizontalFov(horizontalDegrees, this->nearClip);
        setupPerspectiveFromSize(adjustedSize, this->nearClip);
    }
    void setAspectRatio(float aspectRatio) {
        if (aspectRatio <= 0) {
            throw std::invalid_argument("aspect-ratio must be greater than 0");
        }
        Vec2 adjustedSize = Vec2(viewportSize.x, viewportSize.x / this->aspectRatio);
        setupPerspectiveFromSize(adjustedSize, this->nearClip);
    }
    
    void setNearClip(float nearClip) {
        if (nearClip <= 0 || nearClip >= farClip) {
            throw std::invalid_argument("near-clip must be in range of (0, farClip)");
        }
        setupPerspectiveFromSize(this->viewportSize, this->nearClip);
    }
    void setFarClip(float farClip) {
        if (farClip < 0) {
            throw std::invalid_argument("far-clip must be greater than 0");
        }
        this->farClip = farClip;
    }

    void lookAt(const Vec3& target) {
        // if camera is pointed straight up/down, then we choose a new orthogonal vector for computing the
        // right direction (as otherwise the cross product would be zero!)
        Vec3 tempUp{};
        Vec3 directionToTarget = Math::direction(position, target);
        if (!Math::isParallelDirection(directionToTarget, Vec3::up())) {
            tempUp = Vec3::up();
        } else {
            tempUp = directionToTarget.y > 0 ? Vec3::ahead() : Vec3::behind();
        }

        this->forwardDir = directionToTarget;
        this->rightDir   = Math::normalize(Math::cross(this->forwardDir, tempUp));
        this->upDir      = Math::normalize(Math::cross(this->rightDir,   this->forwardDir));
    }
};
inline std::ostream& operator<<(std::ostream& os, const RenderCam& renderCam) {
    os << std::fixed << std::setprecision(2)
       << "RenderCam("
         << "position:(" << renderCam.getPosition() << "), "
         << "Orientation{"
           << "right-axis:("   << renderCam.getRightDir()   << "),"
           << "upward-axis:("  << renderCam.getUpDir()      << "),"
           << "forward-axis:(" << renderCam.getForwardDir() << ")}, "
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
