#pragma once
#include "Math.hpp"
#include "Ray.hpp"


namespace CommonResolutions {
inline constexpr Vec2 SD_240p  = Vec2(  426,  240);
inline constexpr Vec2 SD_360p  = Vec2(  640,  360);
inline constexpr Vec2 SD_480p  = Vec2(  854,  480);
inline constexpr Vec2 HD_720p  = Vec2( 1280,  720);
inline constexpr Vec2 HD_1080p = Vec2( 1920, 1080);
inline constexpr Vec2 HD_2K    = Vec2( 2560, 1440);
inline constexpr Vec2 HD_4K    = Vec2( 3840, 2160);
inline constexpr Vec2 HD_5K    = Vec2( 5120, 2880);
inline constexpr Vec2 HD_8K    = Vec2( 7680, 4320);
inline constexpr Vec2 HD_10K   = Vec2(10328, 7760);
inline constexpr Vec2 HD_12K   = Vec2(12288, 6480);
}

// z-axis-aligned perspective 3d camera (rectangular-pinhole cam without lens)
// note: here, image plane is equivalent to the near plane
// note: fieldOfView is taken as horizontal fov, with the aspect ratio determining the verticalFov
class Camera {
private:
    Vec3 eyePosition_;
    Vec3 rightDir_;
    Vec3 upDir_;
    Vec3 forwardDir_;
    
    float nearClip_;
    float farClip_;
    float aspectRatio_;
    Vec2 fieldOfView_;
    Vec2 viewportSize_;

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
    
public:
    Camera();

    Vec3 viewportToWorld(const Vec3& viewportPosition) const;
    void lookAt(const Vec3& target);
    Ray viewportPointToRay(const Vec3& point) const;

    void setPosition(const Vec3& eyePosition);
    void setOrientation(const Vec3& rightDir, const Vec3& upDir, const Vec3& forwardDir);
    void setNearClip(float nearClip);
    void setFarClip(float farClip);
    void overrideViewportSize(const Vec2& viewportSize, float nearZ, float farZ);
    void setAspectRatio(float aspectRatio);
    void setFieldOfView(float horizontalDegrees);

    constexpr Vec3 position() const;
    constexpr Vec3 rightDir() const;
    constexpr Vec3 upDir() const;
    constexpr Vec3 forwardDir() const;
    constexpr float nearClip() const;
    constexpr float farClip() const;
    constexpr Vec2  viewportSize() const;
    constexpr float aspectRatio() const;
    constexpr float horizontalFieldOfView() const;
    constexpr float verticalFieldOfView() const;

private:
    static Vec2 computeSizeFromHorizontalFov(float horizontalFieldOfView, float distanceToPlane, float aspectRatio);
    void setupPerspectiveFromSize(const Vec2& viewportSize, float nearZ, float farZ);
};
std::ostream& operator<<(std::ostream& os, const Camera& camera);
