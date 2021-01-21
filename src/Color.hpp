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

// note: higher reflectivity index, more reflective it is, with 0 being no reflection, and 1 being max
// todo: add support for refractivity
struct Material {
    Color ambient;
    Color diffuse;
    Color specular;
    float intrinsic;
    float reflectivity;

    Material() = default;
    constexpr Material(const Color& ambient, const Color& diffuse, const Color& specular, float intrinsic, float reflectivity) :
            ambient(ambient),
            diffuse(diffuse),
            specular(specular),
            intrinsic(Math::clamp01(intrinsic)),
            reflectivity(Math::clamp01(reflectivity)) {
        if (!Math::isApproximately(this->intrinsic + this->reflectivity, 1.00f)) {
            throw std::invalid_argument("weighted indices must sum to 1.00");
        }
    }
    constexpr Material(const Color& ambient, const Color& diffuse, const Color& specular, double intrinsic, double reflectivity) :
        Material(ambient, diffuse, specular, static_cast<float>(intrinsic), static_cast<float>(reflectivity)) {}
};
inline std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << std::fixed << std::setprecision(2)
       << "Material("
         << "Colors{"
           << "ambient:"  << material.ambient  << ","
           << "diffuse:"  << material.diffuse  << ","
           << "specular:" << material.specular << "}, "
         << "Weights{"
           << "intrinsic:"    << material.intrinsic    << ","
           << "reflectivity:" << material.reflectivity << "}"
         << ")";
    return os;
}
