#pragma once
#include <iostream>
#include "Math.hpp"


// assumes each color component in range[0.00, 1.00]
// note that brightness or alpha is not (currently) supported
struct Color {
private:
    static constexpr int MIN_8BIT = 0;
    static constexpr int MAX_8BIT = 255;
    static constexpr float INV_MAX_8BIT = 1.0f / MAX_8BIT;
    
    static constexpr float intToFloat(int v)                          { return v * INV_MAX_8BIT;                               }
    static constexpr int   floatToInt(float v)                        { return static_cast<int>(v * MAX_8BIT);                 }
    static constexpr unsigned char extractRedByte(unsigned int hex)   { return static_cast<unsigned char>((hex >> 16) & 0xff); }
    static constexpr unsigned char extractGreenByte(unsigned int hex) { return static_cast<unsigned char>((hex >> 8)  & 0xff); }
    static constexpr unsigned char extractBlueByte(unsigned int hex)  { return static_cast<unsigned char>(hex & 0xff);         }

public:
    float r;
    float g;
    float b;
    
    Color() = default;
    constexpr Color(float  r, float  g, float  b) : r(Math::clamp01(r)),       g(Math::clamp01(g)),       b(Math::clamp01(b))       {}
    constexpr Color(double r, double g, double b) : r(static_cast<float>(r)),  g(static_cast<float>(g)),  b(static_cast<float>(b))  {}
    constexpr Color(int    r, int    g, int    b) : r(intToFloat(r)),          g(intToFloat(g)),          b(intToFloat(b))          {}
    constexpr Color(int hex)                      : r(extractRedByte(hex)),    g(extractGreenByte(hex)),  b(extractBlueByte(hex))   {}
    constexpr Color(const Color& color)           : r(Math::clamp01(color.r)), g(Math::clamp01(color.g)), b(Math::clamp01(color.b)) {}

    constexpr unsigned int getHex() {
        return ( (0xff & static_cast<unsigned char>(floatToInt(r))) << 16 ) |
               ( (0xff & static_cast<unsigned char>(floatToInt(g))) << 8  ) |
               ( (0xff & static_cast<unsigned char>(floatToInt(b)))       );
    }
};
inline constexpr Color operator+(const Color& lhs, const Color& rhs) { return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b); }
inline constexpr Color operator-(const Color& lhs, const Color& rhs) { return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b); }
inline constexpr Color operator*(const Color& lhs, const Color& rhs) { return Color(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b); }
inline constexpr Color operator*(float        lhs, const Color& rhs) { return Color(lhs   * rhs.r, lhs   * rhs.g, lhs   * rhs.b); }
inline constexpr Color operator*(const Color& lhs, float        rhs) { return Color(lhs.r * rhs,   lhs.g * rhs,   lhs.b * rhs);   }
inline constexpr Color operator/(const Color& lhs, float        rhs) { return Color(lhs.r / rhs,   lhs.g / rhs,   lhs.b / rhs);   }
inline std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << color.r << "," << color.g << "," << color.b;
    return os;
}


// color of diffuse/ambient/specular reflectance, as well as weighted intrinsic/reflectivity/refraction
// todo: add proper support for refractivity/refractive index (and maybe emissive color as well??)
class Material {
private:
    Color diffuseColor;
    Color ambientColor;
    Color specularColor;
    float intrinsity;
    float reflectivity;
    float refractivity;
    float specularExponent;

    static constexpr Color DEFAULT_DIFFUSE_COLOR { 0.80f, 0.80f, 0.80f };
    static constexpr Color DEFAULT_AMBIENT_COLOR { 0.20f, 0.20f, 0.20f };
    static constexpr Color DEFAULT_SPECULAR_COLOR{ 0.00f, 0.00f, 0.00f };
    static constexpr float DEFAULT_INTRINSITY        { 1.00f };
    static constexpr float DEFAULT_REFLECTIVITY      { 0.00f };
    static constexpr float DEFAULT_REFRACTIVITY      { 0.00f };
    static constexpr float DEFAULT_SPECULAR_EXPONENT { 0.80f };

public:
    Material(const Color& diffuseColor, const Color& ambientColor, const Color& specularColor,
             float intrinsity, float reflectivity, float refractivity,
             float specularExponent) {
        setColors(diffuseColor, ambientColor, specularColor);
        setWeights(intrinsity, reflectivity, refractivity);
        setShininess(specularExponent);
    }
    Material() : Material(
        DEFAULT_DIFFUSE_COLOR,
        DEFAULT_AMBIENT_COLOR,
        DEFAULT_SPECULAR_COLOR, 
        DEFAULT_INTRINSITY,
        DEFAULT_REFLECTIVITY,
        DEFAULT_REFRACTIVITY,
        DEFAULT_SPECULAR_EXPONENT) {}

    void setColors(const Color& diffuseColor, const Color& ambientColor, const Color& specularColor) {
        this->diffuseColor  = diffuseColor;
        this->ambientColor  = ambientColor;
        this->specularColor = specularColor;
    }
    void setWeights(float intrinsity, float reflectivity, float refractivity=0.00f) {
        if (intrinsity   < 0.00f || intrinsity   > 1.00f ||
            reflectivity < 0.00f || reflectivity > 1.00f ||
            refractivity < 0.00f || refractivity > 1.00f ||
            !Math::isApproximately(intrinsity + reflectivity + refractivity, 1.00f)) {
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
    void setDiffuseColor(const Color& diffuseColor)   { this->diffuseColor  = diffuseColor;  }
    void setAmbientColor(const Color& ambientColor)   { this->ambientColor  = ambientColor;  }
    void setSpecularColor(const Color& specularColor) { this->specularColor = specularColor; }

    float getShininess()            const { return specularExponent; }
    float getIntrinsity()           const { return intrinsity;       }
    float getReflectivity()         const { return reflectivity;     }
    float getRefractivity()         const { return refractivity;     }
    const Color& getDiffuseColor()  const { return diffuseColor;     }
    const Color& getAmbientColor()  const { return ambientColor;     }
    const Color& getSpecularColor() const { return specularColor;    }
};
inline std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << std::fixed << std::setprecision(2)
       << "Material("
         << "Colors{"
           << "ambient:"  << material.getAmbientColor()  << ","
           << "diffuse:"  << material.getDiffuseColor()  << ","
           << "specular:" << material.getSpecularColor() << "}, "
         << "Weights{"
           << "intrinsic:"  << material.getIntrinsity()   << ","
           << "reflective:" << material.getReflectivity() << ","
           << "refractive:" << material.getRefractivity() << "}, "
         << "Shininess{"
           << "specular-exponent:" << material.getShininess() << "}"
         << ")";
    return os;
}
