#include "Lights.hpp"
#include "Math.hpp"
#include "Color.hpp"


PointLight::PointLight(const Vec3& position, const Color& intensity, float constant, float linear, float quadratic)
    : attenuationConstant_ (constant),
      attenuationLinear_   (linear),
      attenuationQuadratic_(quadratic) {
    this->position_  = position;
    this->intensity_ = intensity;
};
PointLight::PointLight(const Vec3& position, const Color& intensity)
    : attenuationConstant_ (DEFAULT_ATTENUATION_CONSTANT),
      attenuationLinear_   (DEFAULT_ATTENUATION_LINEAR),
      attenuationQuadratic_(DEFAULT_ATTENUATION_QUADRATIC) {
    this->position_ = position;
    this->intensity_ = intensity;
}

// compute intensity at given point according to inverse square law with respect to distance and attenuation coefficients
Color PointLight::computeIntensityAtPoint(const Vec3& point) const {
    const float distanceSquared = Math::magnitudeSquared(point - this->position_);
    const Vec3 direction = Math::direction(this->position_, point);
    const float distance  = Math::square(distanceSquared);
    return intensity_ / 
        ((attenuationConstant_) + (attenuationLinear_ * distance) + (attenuationQuadratic_ * distanceSquared));
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

float PointLight::attenuationConstant() const {
    return attenuationConstant_;
}
float PointLight::attenuationLinear() const {
    return attenuationLinear_;
}
float PointLight::attenuationQuadratic() const {
    return attenuationConstant_;
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
