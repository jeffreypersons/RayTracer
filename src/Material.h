#pragma once
#include "Math.hpp"
#include "Color.hpp"


// color of diffuse/ambient/specular reflectance, as well as weighted intrinsic/reflectivity/refraction
class Material {
public:
    Material();
    Material(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor);
    Material(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor,
             float intrinsity, float reflectivity, float refractivity, float shininess);

    void setColors(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor);
    void setWeights(float intrinsity, float reflectivity, float refractivity = 0.00f);
    void setShininess(float shininess);

    Color ambientColor()  const;
    Color diffuseColor()  const;
    Color specularColor() const;
    float intrinsity()    const;
    float reflectivity()  const;
    float refractivity()  const;
    float shininess()     const;

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

    static void validateWeights(float intrinsity, float reflectivity, float refractivity=0.00f);
    static void validateShininess(float shininess);
};
std::ostream& operator<<(std::ostream& os, const Material& material);
