#include "Lights.h"
#include "Math.hpp"
#include "Color.hpp"


PointLight::PointLight(const Vec3& position, const Color& intensity, float constant, float linear, float quadratic) {
    // note that base class members can only be initialized in the body and not our typical initializer list
    this->attenuationConstant_  = constant;
    this->attenuationLinear_    = linear;
    this->attenuationQuadratic_ = quadratic;

    this->position_  = position;
    this->intensity_ = intensity;
};
PointLight::PointLight(const Vec3& position, const Color& intensity) {
    this->attenuationConstant_  = 1.00f;
    this->attenuationLinear_    = 0.00f;
    this->attenuationQuadratic_ = 0.00f;

    this->position_ = position;
    this->intensity_ = intensity;
}

// compute intensity at given point according to inverse square law with respect to distance and attenuation coefficients
Color PointLight::computeIntensityAtPoint(const Vec3& point) const {
    float distanceSquared = Math::magnitudeSquared(point - this->position_);
    Vec3 direction = Math::direction(this->position_, point);
    float distance  = Math::square(distanceSquared);
    return intensity_ / ((attenuationConstant_) + (attenuationLinear_ * distance) + (attenuationQuadratic_ * distanceSquared));
}

// attenuation coefficients sum to create a falloff from source in range [0.0, 1.0],
// with 0 being least amount of fall off and 1 being the most
void PointLight::setAttenuation(float constant, float linear, float quadratic) {
    if (constant < 0.00f || linear < 0.00f || quadratic < 0.00f ||
            Math::isApproximately(constant + linear + quadratic, 0.00f)) {
        throw std::invalid_argument("attenuation coefficients must be greater than zero and sum to greater than zero");
    }
    this->attenuationConstant_  = constant;
    this->attenuationLinear_    = linear;
    this->attenuationQuadratic_ = quadratic;
}

constexpr float PointLight::attenuationConstant() const {
    return attenuationConstant_;
}
constexpr float PointLight::attenuationLinear() const {
    return attenuationLinear_;
}
constexpr float PointLight::attenuationQuadratic() const {
    return attenuationConstant_;
}

std::string PointLight::description() const {
    std::stringstream ss;
    ss << "PointLight("
         << "position:("  << position()  << "),"
         << "intensity:(" << intensity() << "),"
       << "Attenuation{"
         << "constant:"  << attenuationConstant()  << ","
         << "linear:"    << attenuationLinear()    << ","
         << "quadratic:" << attenuationQuadratic() << "}"
       << ")";
    return ss.str();
}
