#include "Camera.h"
#include "Math.hpp"
#include "Ray.hpp"


Camera::Camera() {
    Vec2 size = computeSizeFromHorizontalFov(DEFAULT_FIELD_OF_VIEW, DEFAULT_NEAR_CLIP, DEFAULT_ASPECT_RATIO);
    setPosition(DEFAULT_POSITION);
    setOrientation(DEFAULT_RIGHT_DIR, DEFAULT_UP_DIR, DEFAULT_FORWARD_DIR);
    setupPerspectiveFromSize(size, DEFAULT_NEAR_CLIP, DEFAULT_FAR_CLIP);
}

void Camera::lookAt(const Vec3& target) {
    // if camera is pointed straight up/down, then we choose a new orthogonal vector for computing the
    // right direction (as otherwise the cross product would be zero!)
    Vec3 tempUp{};
    const Vec3 directionToTarget = Math::direction(eyePosition_, target);
    if (!Math::isParallelDirection(directionToTarget, Vec3::up())) {
        tempUp = Vec3::up();
    } else {
        tempUp = directionToTarget.y > 0 ? Vec3::ahead() : Vec3::behind();
    }

    this->forwardDir_ = directionToTarget;
    this->rightDir_   = Math::normalize(Math::cross(this->forwardDir_, tempUp));
    this->upDir_      = Math::normalize(Math::cross(this->rightDir_,   this->forwardDir_));
}

// map point from viewport coordinates to worldspace coordinates
// note: point in viewport if between bottom-left of near-plane (0,0,0) and top right-right of far-plane (1,1,1)
constexpr Vec3 Camera::viewportToWorld(const Vec3& point) const {
    const Vec3 projectedCenter  = eyePosition_ + ((nearClip_ + (point.z * farClip_)) * forwardDir_);
    const Vec3 offsetInRightDir = ((point.x - 0.50f) * viewportSize_.x * rightDir_);
    const Vec3 offsetInUpDir    = ((point.y - 0.50f) * viewportSize_.y * upDir_);
    return projectedCenter + offsetInRightDir + offsetInUpDir;
}
// map point from worldspace coordinates to viewport coordinates
// note: point in viewport if between bottom-left of near-plane (0,0,0) and top right-right of far-plane (1,1,1)
constexpr Vec3 Camera::worldToViewport(const Vec3& point) const {
    const Vec3 frustumMin = viewportToWorld(Vec3(0, 0, 0));
    const Vec3 frustumMax = viewportToWorld(Vec3(1, 1, 1));
    return Vec3{
        Math::scaleToRange(point.x, frustumMin.x, frustumMax.x, 0.00f, 1.00f),
        Math::scaleToRange(point.y, frustumMin.y, frustumMax.y, 0.00f, 1.00f),
        Math::scaleToRange(point.z, frustumMin.z, frustumMax.z, 0.00f, 1.00f)
    };
}
// convenience method for returning return ray directed from camera position to given point
Ray Camera::viewportPointToRay(const Vec3& point) const {
    return Ray(eyePosition_, Math::direction(eyePosition_, viewportToWorld(point)));
}

void Camera::setPosition(const Vec3& eyePosition) {
    this->eyePosition_ = eyePosition;
}
void Camera::setOrientation(const Vec3& rightDir, const Vec3& upDir, const Vec3& forwardDir) {
    if (!Math::isNormalized(forwardDir) || !Math::isNormalized(rightDir) || !Math::isNormalized(upDir)) {
        throw std::invalid_argument("all given orientation vectors must be normalized");
    }
    if (!Math::isOrthogonal(rightDir, forwardDir) || !Math::isOrthogonal(forwardDir, upDir)) {
        throw std::invalid_argument("all given orientation vectors must be orthogonal to one another");
    }
    this->rightDir_   = rightDir;
    this->upDir_      = upDir;
    this->forwardDir_ = forwardDir;
}
void Camera::setNearClip(float nearClip) {
    if (nearClip_ <= 0 || nearClip >= farClip_) {
        throw std::invalid_argument("near-clip must be in range of (0, farClip)");
    }
    setupPerspectiveFromSize(this->viewportSize_, nearClip, this->farClip_);
}
void Camera::setFarClip(float farClip) {
    if (farClip < 0) {
        throw std::invalid_argument("far-clip must be greater than 0");
    }
    setupPerspectiveFromSize(this->viewportSize_, this->nearClip_, this->farClip_);
}
// overrides field of view, aspect ratio, and clipping planes to match given viewport dimensions
void Camera::overrideViewportSize(const Vec2& viewportSize, float nearZ, float farZ) {
    if (viewportSize.x <= 0 || viewportSize.y <= 0) {
        throw std::invalid_argument("viewport size dimensions must each be greater than 0");
    }
    setupPerspectiveFromSize(viewportSize, nearZ, farZ);
}
void Camera::setAspectRatio(float aspectRatio) {
    if (aspectRatio <= 0) {
        throw std::invalid_argument("aspect-ratio must be greater than 0");
    }
    Vec2 adjustedSize{ viewportSize_.x, viewportSize_.x / aspectRatio };
    setupPerspectiveFromSize(adjustedSize, this->nearClip_, this->farClip_);
}
void Camera::setFieldOfView(float horizontalDegrees) {
    if (horizontalDegrees <= MIN_FIELD_OF_VIEW || horizontalDegrees >= MAX_FIELD_OF_VIEW) {
        throw std::invalid_argument("field-of-view must be in range (0, 180)");
    }
    Vec2 adjustedSize = computeSizeFromHorizontalFov(horizontalDegrees, this->nearClip_, this->aspectRatio_);
    setupPerspectiveFromSize(adjustedSize, this->nearClip_, this->farClip_);
}

constexpr Vec3 Camera::position() const {
    return eyePosition_;
}
constexpr Vec3 Camera::rightDir() const {
    return rightDir_;
}
constexpr Vec3 Camera::upDir() const {
    return upDir_;
}
constexpr Vec3 Camera::forwardDir() const {
    return forwardDir_;
}
constexpr float Camera::nearClip() const {
    return nearClip_;
}
constexpr float Camera::farClip() const {
    return farClip_;
}
constexpr Vec2 Camera::viewportSize() const {
    return viewportSize_;
}
constexpr float Camera::aspectRatio() const {
    return aspectRatio_;
}
constexpr float Camera::horizontalFieldOfView() const {
    return fieldOfView_.x;
}
constexpr float Camera::verticalFieldOfView() const {
    return fieldOfView_.y;
}

// find viewport size by solving for the triangular len (width/height) in the equation `tan(fov * 0.5) = (0.5 * len) / dist`
Vec2 Camera::computeSizeFromHorizontalFov(float horizontalFieldOfView, float distanceToPlane, float aspectRatio) {
    const float viewportWidth  = 2.00f * distanceToPlane * Math::tan(0.50f * horizontalFieldOfView);
    const float viewportHeight = viewportWidth / aspectRatio;
    return Vec2(viewportWidth, viewportHeight);
}
// find field of view by solving for the fov in the equation `tan(fov * 0.5) = (0.5 * len) / dist
void Camera::setupPerspectiveFromSize(const Vec2& viewportSize, float nearZ, float farZ) {
    this->nearClip_      = nearZ;
    this->farClip_       = farZ;
    this->viewportSize_  = viewportSize;
    this->aspectRatio_   = viewportSize.x / viewportSize.y;
    this->fieldOfView_.x = 2.00f * Math::atan(viewportSize.x / (2.00f * nearZ));
    this->fieldOfView_.y = 2.00f * Math::atan(viewportSize.y / (2.00f * nearZ));
}

std::ostream& operator<<(std::ostream& os, const Camera& camera) {
    os << "Camera("
         << "position:(" << camera.position() << "), "
         << "Orientation{"
           << "right-axis:("   << camera.rightDir()   << "),"
           << "upward-axis:("  << camera.upDir()      << "),"
           << "forward-axis:(" << camera.forwardDir() << ")}, "
         << "ImagePlane{"
           << "aspect-ratio:"    << camera.aspectRatio()           << ","
           << "field-of-view-x:" << camera.horizontalFieldOfView() << ","
           << "field-of-view-y:" << camera.verticalFieldOfView()   << ","
           << "viewport-width:"  << camera.viewportSize().x        << ","
           << "viewport-height:" << camera.viewportSize().y        << "}, "
         << "Clipping{"
           << "near-plane-z:" << camera.nearClip() << ","
           << "far-plane-z:"  << camera.farClip()  << "}"
       << ")";
    return os;
}
