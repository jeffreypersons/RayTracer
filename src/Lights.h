#pragma once
#include "Math.hpp"
#include "Color.hpp"


class ILight {
protected:
    Vec3 position_{};
    Color intensity_{};
    
public:
    virtual ~ILight() = default;
    virtual Color computeIntensityAtPoint(const Vec3& point) const = 0;
    virtual std::string description() const = 0;

    constexpr Vec3  position()  const { return position_;  }
    constexpr Color intensity() const { return intensity_; }
};
inline std::ostream& operator<<(std::ostream& os, const ILight& light) {
    os << light.description();
    return os;
}


class PointLight final : public virtual ILight {
private:
    float attenuationConstant_;
    float attenuationLinear_;
    float attenuationQuadratic_;

public:
    PointLight(const Vec3& position, const Color& intensity, float constant, float linear, float quadratic);
    PointLight(const Vec3& position, const Color& intensity);

    virtual Color computeIntensityAtPoint(const Vec3& point) const override;
    virtual std::string description() const override;

    void setAttenuation(float constant, float linear, float quadratic);

    constexpr float attenuationConstant()  const;
    constexpr float attenuationLinear()    const;
    constexpr float attenuationQuadratic() const;
};
