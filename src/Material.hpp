#pragma once
#include <iostream>
#include "Math.hpp"
#include "Color.hpp"


// color of diffuse/ambient/specular reflectance, as well as weighted intrinsic/reflectivity/refraction
class Material {
private:
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    float intrinsity;
    float reflectivity;
    float refractivity;
    float specularExponent;

    static constexpr Color DEFAULT_AMBIENT_COLOR  { 0.20f, 0.20f, 0.20f };
    static constexpr Color DEFAULT_DIFFUSE_COLOR  { 0.80f, 0.80f, 0.80f };
    static constexpr Color DEFAULT_SPECULAR_COLOR { 0.00f, 0.00f, 0.00f };
    static constexpr float DEFAULT_INTRINSITY   { 1.00f };
    static constexpr float DEFAULT_REFLECTIVITY { 0.00f };
    static constexpr float DEFAULT_REFRACTIVITY { 0.00f };
    static constexpr float DEFAULT_SPECULAR_EXPONENT { 0.80f };

public:
    Material(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor,
             float intrinsity, float reflectivity, float refractivity,
             float specularExponent) {
        setColors(ambientColor, diffuseColor, specularColor);
        setWeights(intrinsity, reflectivity, refractivity);
        setShininess(specularExponent);
    }
    Material() : Material(
        DEFAULT_AMBIENT_COLOR,
        DEFAULT_DIFFUSE_COLOR,
        DEFAULT_SPECULAR_COLOR,
        DEFAULT_INTRINSITY,
        DEFAULT_REFLECTIVITY,
        DEFAULT_REFRACTIVITY,
        DEFAULT_SPECULAR_EXPONENT) {}

    void setColors(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor) {
        this->ambientColor  = ambientColor;
        this->diffuseColor  = diffuseColor;
        this->specularColor = specularColor;
    }
    void setWeights(float intrinsity, float reflectivity, float refractivity = 0.00f) {
        if (!Math::isApproximately(intrinsity + reflectivity + refractivity, 1.00f) ||
                intrinsity   < 0.00f || intrinsity   > 1.00f ||
                reflectivity < 0.00f || reflectivity > 1.00f ||
                refractivity < 0.00f || refractivity > 1.00f) {
            throw std::invalid_argument("weights must be in range[0.0, 1.0] and sum to 1.0");
        }
        this->intrinsity   = intrinsity;
        this->reflectivity = reflectivity;
        this->refractivity = refractivity;
    }
    void setShininess(float specularExponent) {
        if (specularExponent <= 0.00f) {
            throw std::invalid_argument("specular exponent must be greater than zero");
        }
        this->specularExponent = specularExponent;
    }
    void setAmbientColor(const Color& ambientColor)   { this->ambientColor  = ambientColor;  }
    void setDiffuseColor(const Color& diffuseColor)   { this->diffuseColor  = diffuseColor;  }
    void setSpecularColor(const Color& specularColor) { this->specularColor = specularColor; }

    float getShininess()            const { return specularExponent; }
    float getIntrinsity()           const { return intrinsity;       }
    float getReflectivity()         const { return reflectivity;     }
    float getRefractivity()         const { return refractivity;     }
    const Color& getAmbientColor()  const { return ambientColor;     }
    const Color& getDiffuseColor()  const { return diffuseColor;     }
    const Color& getSpecularColor() const { return specularColor;    }
};
inline std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << std::fixed << std::setprecision(2)
       << "Material("
         << "Colors{"
           << "ambient:("  << material.getAmbientColor()  << "),"
           << "diffuse:("  << material.getDiffuseColor()  << "),"
           << "specular:(" << material.getSpecularColor() << ")}, "
         << "Weights{"
           << "intrinsic:"  << material.getIntrinsity()   << ","
           << "reflective:" << material.getReflectivity() << ","
           << "refractive:" << material.getRefractivity() << "}, "
         << "Shininess{"
           << "specular-exponent:" << material.getShininess() << "}"
       << ")";
    return os;
}
