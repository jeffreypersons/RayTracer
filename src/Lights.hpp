#pragma once
#include "Math.hpp"


// represents a point light in 3d space
class PointLight {
private:
    Vec3 position;
    Color intensity;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

public:
    PointLight(const Vec3& position, const Color& intensity, float constant, float linear, float quadratic) :
        position(position),
        intensity(intensity),
        attenuationConstant(constant),
        attenuationLinear(linear),
        attenuationQuadratic(quadratic) {}

    PointLight(const Vec3& position, const Color& intensity) : PointLight(position, intensity, 1, 0, 0) {}
    
    // compute intensity at given point according to inverse square law with respect to distance and attenuation coefficients
    Color computeIntensityAtPoint(const Vec3& point) const {
        float distanceSquared = Math::magnitudeSquared(point - this->position);
        Vec3 direction = Math::direction(this->position, point);
        float distance  = Math::square(distanceSquared);
        return intensity / ((attenuationConstant) + (attenuationLinear * distance) + (attenuationQuadratic * distanceSquared));
    }
    
    void setPosition(const Vec3& position)    { this->position  = position;  }
    void setIntensity(const Color& intensity) { this->intensity = intensity; }

    // falloff in range [0.0, 1.0], with 0 being least amount of fall off and 1 being the most
    // todo: add PROPER range checks with exceptions and then call in constructor...(research valid values)
    void setAttenuation(float constant, float linear, float quadratic) {
        if (constant < 0.00f || linear < 0.00f || quadratic < 0.00f ||
                Math::isApproximately(constant + linear + quadratic, 0.00f)) {
            throw std::invalid_argument("attenuation coefficients must be greater than zero and sum to greater than zero");
        }
        this->attenuationConstant  = constant;
        this->attenuationLinear    = linear;
        this->attenuationQuadratic = quadratic;
    }

    Vec3 getPosition()              const { return position;            }
    Color getIntensity()            const { return intensity;           }
    float getAttenuationConstant()  const { return attenuationConstant; }
    float getAttenuationLinear()    const { return attenuationLinear;   }
    float getAttenuationQuadratic() const { return attenuationConstant; }
};
inline std::ostream& operator<<(std::ostream& os, const PointLight& light) {
    os << std::fixed << std::setprecision(2)
       << "PointLight("
         << "position:("  << light.getPosition()  << "),"
         << "intensity:(" << light.getIntensity() << "),"
       << "Attenuation{"
         << "constant:"  << light.getAttenuationConstant()  << ","
         << "linear:"    << light.getAttenuationLinear()    << ","
         << "quadratic:" << light.getAttenuationQuadratic() << "}"
       << ")";
    return os;
}
