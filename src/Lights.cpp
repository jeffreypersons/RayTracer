#include "Lights.h"
#include "Math.hpp"
#include "Color.hpp"


PointLight::PointLight(const Vec3& position, const Color& intensity, float constant, float linear, float quadratic) {
    // note that base class members can only be initialized in the body and not our typical initializer list
    this->position  = position;
    this->intensity = intensity;
    this->attenuationConstant  = constant;
    this->attenuationLinear    = linear;
    this->attenuationQuadratic = quadratic;
};
PointLight::PointLight(const Vec3& position, const Color& intensity) {
    this->position  = position;
    this->intensity = intensity;
    this->attenuationConstant  = 1.00f;
    this->attenuationLinear    = 0.00f;
    this->attenuationQuadratic = 0.00f;
}

// compute intensity at given point according to inverse square law with respect to distance and attenuation coefficients
Color PointLight::computeIntensityAtPoint(const Vec3& point) const {
    float distanceSquared = Math::magnitudeSquared(point - this->position);
    Vec3 direction = Math::direction(this->position, point);
    float distance  = Math::square(distanceSquared);
    return intensity / ((attenuationConstant) + (attenuationLinear * distance) + (attenuationQuadratic * distanceSquared));
}

// attenuation coefficients sum to create a falloff from source in range [0.0, 1.0],
// with 0 being least amount of fall off and 1 being the most
void PointLight::setAttenuation(float constant, float linear, float quadratic) {
    if (constant < 0.00f || linear < 0.00f || quadratic < 0.00f ||
            Math::isApproximately(constant + linear + quadratic, 0.00f)) {
        throw std::invalid_argument("attenuation coefficients must be greater than zero and sum to greater than zero");
    }
    this->attenuationConstant  = constant;
    this->attenuationLinear    = linear;
    this->attenuationQuadratic = quadratic;
}

float PointLight::getAttenuationConstant() const {
    return attenuationConstant;
}
float PointLight::getAttenuationLinear() const {
    return attenuationLinear;
}
float PointLight::getAttenuationQuadratic() const {
    return attenuationConstant;
}

std::string PointLight::getDescription() const {
    std::stringstream ss;
    ss << "PointLight("
         << "position:("  << getPosition()  << "),"
         << "intensity:(" << getIntensity() << "),"
       << "Attenuation{"
         << "constant:"  << getAttenuationConstant()  << ","
         << "linear:"    << getAttenuationLinear()    << ","
         << "quadratic:" << getAttenuationQuadratic() << "}"
       << ")";
    return ss.str();
}
