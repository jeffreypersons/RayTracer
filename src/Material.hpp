#pragma once
#include <iostream>
#include "Math.hpp"
#include "Color.hpp"


// color of diffuse/ambient/specular reflectance, as well as weighted intrinsic/reflectivity/refraction
class Material {
private:
    Color ambientColor_;
    Color diffuseColor_;
    Color specularColor_;
    float intrinsity_;
    float reflectivity_;
    float refractivity_;
    float shininess_;

    static constexpr Color DEFAULT_AMBIENT_COLOR  { 0.20f, 0.20f, 0.20f };
    static constexpr Color DEFAULT_DIFFUSE_COLOR  { 0.80f, 0.80f, 0.80f };
    static constexpr Color DEFAULT_SPECULAR_COLOR { 0.00f, 0.00f, 0.00f };
    static constexpr float DEFAULT_INTRINSITY        { 1.00f };
    static constexpr float DEFAULT_REFLECTIVITY      { 0.00f };
    static constexpr float DEFAULT_REFRACTIVITY      { 0.00f };
    static constexpr float DEFAULT_SPECULAR_EXPONENT { 1.00f };

public:
    constexpr Material()
        : Material(
            DEFAULT_AMBIENT_COLOR,
            DEFAULT_DIFFUSE_COLOR,
            DEFAULT_SPECULAR_COLOR,
            DEFAULT_INTRINSITY,
            DEFAULT_REFLECTIVITY,
            DEFAULT_REFRACTIVITY,
            DEFAULT_SPECULAR_EXPONENT)
    {}
    constexpr Material(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor)
        : Material(
            ambientColor,
            diffuseColor,
            specularColor,
            DEFAULT_INTRINSITY,
            DEFAULT_REFLECTIVITY,
            DEFAULT_REFRACTIVITY,
            DEFAULT_SPECULAR_EXPONENT)
    {}
    constexpr Material(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor,
             float intrinsity, float reflectivity, float refractivity,
             float shininess) 
        : ambientColor_ (ambientColor),
          diffuseColor_ (diffuseColor),
          specularColor_(specularColor),
          intrinsity_   (intrinsity),
          reflectivity_ (reflectivity),
          refractivity_ (refractivity),
          shininess_    (shininess) {
        validateWeights(intrinsity, reflectivity, refractivity);
        validateShininess(shininess);
    }

    constexpr void setColors(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor) {
        this->ambientColor_  = ambientColor;
        this->diffuseColor_  = diffuseColor;
        this->specularColor_ = specularColor;
    }
    constexpr void setWeights(float intrinsity, float reflectivity, float refractivity=0.00f) {
        validateWeights(intrinsity, reflectivity, refractivity);
        this->intrinsity_   = intrinsity;
        this->reflectivity_ = reflectivity;
        this->refractivity_ = refractivity;
    }
    constexpr void setShininess(float shininess) {
        validateShininess(shininess);
        this->shininess_ = shininess;
    }

    constexpr Color ambientColor()  const { return ambientColor_;  }
    constexpr Color diffuseColor()  const { return diffuseColor_;  }
    constexpr Color specularColor() const { return specularColor_; }
    constexpr float intrinsity()    const { return intrinsity_;    }
    constexpr float reflectivity()  const { return reflectivity_;  }
    constexpr float refractivity()  const { return refractivity_;  }
    constexpr float shininess()     const { return shininess_;     }

private:
    static constexpr void validateWeights(float intrinsity, float reflectivity, float refractivity=0.00f) {
        if (!Math::isApproximately(intrinsity + reflectivity + refractivity, 1.00f) ||
                intrinsity   < 0.00f || intrinsity   > 1.00f ||
                reflectivity < 0.00f || reflectivity > 1.00f ||
                refractivity < 0.00f || refractivity > 1.00f) {
            throw std::invalid_argument("weights must be in range[0.0, 1.0] and sum to 1.0");
        }
    }
    static constexpr void validateShininess(float shininess) {
        if (shininess <= 0.00f) {
            throw std::invalid_argument("shininess must be greater than zero");
        }
    }
};
inline std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << "Material("
         << "Colors{"
           << "ambient:("  << material.ambientColor()  << "),"
           << "diffuse:("  << material.diffuseColor()  << "),"
           << "specular:(" << material.specularColor() << ")}, "
         << "Weights{"
           << "intrinsic:"  << material.intrinsity()   << ","
           << "reflective:" << material.reflectivity() << ","
           << "refractive:" << material.refractivity() << "}, "
         << "Shininess{"
           << "specular-exponent:" << material.shininess() << "}"
       << ")";
    return os;
}
