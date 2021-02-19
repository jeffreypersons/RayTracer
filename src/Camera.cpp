#include "Camera.h"
#include "Math.hpp"
#include "Ray.hpp"


Camera::Camera() {
    Vec2 size = computeSizeFromHorizontalFov(DEFAULT_FIELD_OF_VIEW, DEFAULT_NEAR_CLIP, DEFAULT_ASPECT_RATIO);
    setPosition(DEFAULT_POSITION);
    setOrientation(DEFAULT_RIGHT_DIR, DEFAULT_UP_DIR, DEFAULT_FORWARD_DIR);
    setupPerspectiveFromSize(size, DEFAULT_NEAR_CLIP, DEFAULT_FAR_CLIP);
}

// compute position in world relative to bottom-left of viewport
// note that points are in the view frustum if {u: [0, 1], v: [0, 1], w: [nearClip, farClip]
Vec3 Camera::viewportToWorld(const Vec3& viewportPosition) const {
    float centeredU = viewportPosition.x - 0.50f;
    float centeredV = viewportPosition.y - 0.50f;
    float centeredW = viewportPosition.z;
    Vec3 viewportCenter   = eyePosition + (centeredW * forwardDir);
    Vec3 offsetInRightDir = (centeredU * viewportSize.x * rightDir);
    Vec3 offsetInUpDir    = (centeredV * viewportSize.y * upDir);
    return viewportCenter + offsetInRightDir + offsetInUpDir;
}
void Camera::lookAt(const Vec3& target) {
    // if camera is pointed straight up/down, then we choose a new orthogonal vector for computing the
    // right direction (as otherwise the cross product would be zero!)
    Vec3 tempUp{};
    Vec3 directionToTarget = Math::direction(eyePosition, target);
    if (!Math::isParallelDirection(directionToTarget, Vec3::up())) {
        tempUp = Vec3::up();
    }
    else {
        tempUp = directionToTarget.y > 0 ? Vec3::ahead() : Vec3::behind();
    }

    this->forwardDir = directionToTarget;
    this->rightDir = Math::normalize(Math::cross(this->forwardDir, tempUp));
    this->upDir = Math::normalize(Math::cross(this->rightDir, this->forwardDir));
}

void Camera::setPosition(const Vec3& eyePosition) {
    this->eyePosition = eyePosition;
}
void Camera::setOrientation(const Vec3& rightDir, const Vec3& upDir, const Vec3& forwardDir) {
    if (!Math::isNormalized(forwardDir) || !Math::isNormalized(rightDir) || !Math::isNormalized(upDir)) {
        throw std::invalid_argument("all given orientation vectors must be normalized");
    }
    if (!Math::isOrthogonal(rightDir, forwardDir) || !Math::isOrthogonal(forwardDir, upDir)) {
        throw std::invalid_argument("all given orientation vectors must be orthogonal to one another");
    }
    this->rightDir = rightDir;
    this->upDir = upDir;
    this->forwardDir = forwardDir;
}
void Camera::setNearClip(float nearClip) {
    if (nearClip <= 0 || nearClip >= farClip) {
        throw std::invalid_argument("near-clip must be in range of (0, farClip)");
    }
    setupPerspectiveFromSize(this->viewportSize, nearClip, this->farClip);
}
void Camera::setFarClip(float farClip) {
    if (farClip < 0) {
        throw std::invalid_argument("far-clip must be greater than 0");
    }
    setupPerspectiveFromSize(this->viewportSize, this->nearClip, this->farClip);
}
// overrides field of view, aspect ratio, and clipping planes to match given viewport dimensions
// compute field of view by solving for the fov in the equation `tan(fov * 0.5) = (0.5 * width) / dist`
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
    Vec2 adjustedSize{ viewportSize.x, viewportSize.x / aspectRatio };
    setupPerspectiveFromSize(adjustedSize, this->nearClip, this->farClip);
}
void Camera::setFieldOfView(float horizontalDegrees) {
    if (horizontalDegrees <= MIN_FIELD_OF_VIEW || horizontalDegrees >= MAX_FIELD_OF_VIEW) {
        throw std::invalid_argument("field-of-view must be in range (0, 180)");
    }
    Vec2 adjustedSize = computeSizeFromHorizontalFov(horizontalDegrees, this->nearClip, this->aspectRatio);
    setupPerspectiveFromSize(adjustedSize, this->nearClip, this->farClip);
}

constexpr Vec3 Camera::getPosition() const {
    return eyePosition;
}
constexpr Vec3 Camera::getRightDir() const {
    return rightDir;
}
constexpr Vec3 Camera::getUpDir() const {
    return upDir;
}
constexpr Vec3 Camera::getForwardDir() const {
    return forwardDir;
}
constexpr float Camera::getNearClip() const {
    return nearClip;
}
constexpr float Camera::getFarClip() const {
    return farClip;
}
constexpr Vec2 Camera::getViewportSize() const {
    return viewportSize;
}
constexpr float Camera::getAspectRatio() const {
    return aspectRatio;
}
constexpr float Camera::getHorizontalFieldOfView() const {
    return fieldOfView.x;
}
constexpr float Camera::getVerticalFieldOfView() const {
    return fieldOfView.y;
}

// find viewport size by solving for the triangular len (width/height) in the equation `tan(fov * 0.5) = (0.5 * len) / dist`
Vec2 Camera::computeSizeFromHorizontalFov(float horizontalFieldOfView, float distanceToPlane, float aspectRatio) {
    float viewportWidth  = 2.00f * distanceToPlane * Math::tan(0.50f * horizontalFieldOfView);
    float viewportHeight = viewportWidth / aspectRatio;
    return Vec2(viewportWidth, viewportHeight);
}
// find field of view by solving for the fov in the equation `tan(fov * 0.5) = (0.5 * len) / dist
void Camera::setupPerspectiveFromSize(const Vec2& viewportSize, float nearZ, float farZ) {
    this->nearClip = nearZ;
    this->farClip = farZ;
    this->viewportSize = viewportSize;
    this->aspectRatio = viewportSize.x / viewportSize.y;
    this->fieldOfView.x = 2.00f * Math::atan(viewportSize.x / (2.00f * nearZ));
    this->fieldOfView.y = 2.00f * Math::atan(viewportSize.y / (2.00f * nearZ));
}

std::ostream& operator<<(std::ostream& os, const Camera& camera) {
    os << "Camera("
         << "position:(" << camera.getPosition() << "), "
         << "Orientation{"
           << "right-axis:("   << camera.getRightDir()   << "),"
           << "upward-axis:("  << camera.getUpDir()      << "),"
           << "forward-axis:(" << camera.getForwardDir() << ")}, "
         << "ImagePlane{"
           << "aspect-ratio:"    << camera.getAspectRatio()           << ","
           << "field-of-view-x:" << camera.getHorizontalFieldOfView() << ","
           << "field-of-view-y:" << camera.getVerticalFieldOfView()   << ","
           << "viewport-width:"  << camera.getViewportSize().x        << ","
           << "viewport-height:" << camera.getViewportSize().y        << "}, "
         << "Clipping{"
           << "near-plane-z:" << camera.getNearClip() << ","
           << "far-plane-z:"  << camera.getFarClip() << "}"
       << ")";
    return os;
}
