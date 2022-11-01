#pragma once
#include "Math.hpp"
#include "Color.hpp"

// color of diffuse reflectance, as well as weighted intrinsic/reflectivity/refraction
class Material {
public:
    Material();
    Material(const Color& diffuseColor);
    Material(const Color& diffuseColor, float reflectivity, float shininess, float transparency, float indexOfRefraction);

    Color diffuseColor()       const;
    float reflectivity()       const;
    float shininess()          const;
    float transparency()      const;
    float indexOfRefraction()  const;

    void setColor(const Color& diffuseColor);
    void setReflectivity(float reflectivity);
    void setShininess(float shininess);
    void setTransparency(float transparency);
    void setIndexOfRefraction(float indexOfRefraction);

private:
    Color diffuseColor_;
    float reflectivity_;
    float shininess_;
    float transparency_;
    float indexOfRefraction_;

    static constexpr Color DEFAULT_DIFFUSE_COLOR       { 0.80f, 0.80f, 0.80f };
    static constexpr float DEFAULT_REFLECTIVITY        { 0.00f };
    static constexpr float DEFAULT_SPECULAR_EXPONENT   { 1.00f };
    static constexpr float DEFAULT_TRANSPARENCY        { 0.00f };
    static constexpr float DEFAULT_INDEX_OF_REFRACTION { 1.00f };

    static void validateReflectivity(float reflectivity);
    static void validateShininess(float shininess);
    static void validateTransparency(float transparency);
    static void validateIndexOfRefraction(float indexOfRefraction);
};
std::ostream& operator<<(std::ostream& os, const Material& material);


namespace MaterialSample {
    extern const Material matteBlack;
    extern const Material matteGray;
    extern const Material reflectiveRed;
    extern const Material reflectiveGreen;
    extern const Material reflectiveBlue;
    extern const Material transparentRed;
    extern const Material transparentGreen;
    extern const Material transparentBlue;
}
