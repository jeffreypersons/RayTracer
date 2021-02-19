#pragma once
#include "Math.hpp"
#include "Color.hpp"


class ILight {
protected:
    Vec3 position{};
    Color intensity{};

public:
    ~ILight() {}

    virtual Color computeIntensityAtPoint(const Vec3& point) const = 0;
    virtual std::string getDescription() const = 0;

    void setPosition(const Vec3& position)    { this->position  = position;  }
    void setIntensity(const Color& intensity) { this->intensity = intensity; }

    Vec3  getPosition()  const { return position;  }
    Color getIntensity() const { return intensity; }
};
inline std::ostream& operator<<(std::ostream& os, const ILight& light) {
    os << light.getDescription();
    return os;
}


class PointLight : public ILight {
private:
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

public:
    PointLight(const Vec3& position, const Color& intensity, float constant, float linear, float quadratic);
    PointLight(const Vec3& position, const Color& intensity);

    virtual Color computeIntensityAtPoint(const Vec3& point) const override;
    virtual std::string getDescription() const override;

    void setAttenuation(float constant, float linear, float quadratic);

    float getAttenuationConstant()  const;
    float getAttenuationLinear()    const;
    float getAttenuationQuadratic() const;
};
