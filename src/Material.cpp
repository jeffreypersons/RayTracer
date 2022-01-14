#include "Material.hpp"
#include "Math.hpp"
#include "Color.hpp"
#include <iostream>


Material::Material()
    : Material(
        DEFAULT_AMBIENT_COLOR,
        DEFAULT_DIFFUSE_COLOR,
        DEFAULT_SPECULAR_COLOR,
        DEFAULT_INTRINSITY,
        DEFAULT_REFLECTIVITY,
        DEFAULT_REFRACTIVITY,
        DEFAULT_SPECULAR_EXPONENT) {}

Material::Material(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor)
    : Material(
        ambientColor,
        diffuseColor,
        specularColor,
        DEFAULT_INTRINSITY,
        DEFAULT_REFLECTIVITY,
        DEFAULT_REFRACTIVITY,
        DEFAULT_SPECULAR_EXPONENT) {}

Material::Material(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor,
                   float intrinsity, float reflectivity, float refractivity, float shininess)
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


Color Material::ambientColor() const {
    return ambientColor_;
}

Color Material::diffuseColor() const {
    return diffuseColor_;
}

Color Material::specularColor() const {
    return specularColor_;
}

float Material::intrinsity() const {
    return intrinsity_;
}

float Material::reflectivity() const {
    return reflectivity_;
}

float Material::refractivity() const {
    return refractivity_;
}

float Material::shininess() const {
    return shininess_;
}


void Material::setColors(const Color& ambientColor, const Color& diffuseColor, const Color& specularColor) {
    this->ambientColor_ = ambientColor;
    this->diffuseColor_ = diffuseColor;
    this->specularColor_ = specularColor;
}

void Material::setWeights(float intrinsity, float reflectivity, float refractivity) {
    validateWeights(intrinsity, reflectivity, refractivity);
    this->intrinsity_ = intrinsity;
    this->reflectivity_ = reflectivity;
    this->refractivity_ = refractivity;
}

void Material::setShininess(float shininess) {
    validateShininess(shininess);
    this->shininess_ = shininess;
}


void Material::validateWeights(float intrinsity, float reflectivity, float refractivity) {
    if (!Math::isApproximately(intrinsity + reflectivity + refractivity, 1.00f) ||
          intrinsity   < 0.00f || intrinsity   > 1.00f ||
          reflectivity < 0.00f || reflectivity > 1.00f ||
          refractivity < 0.00f || refractivity > 1.00f) {
        throw std::invalid_argument("weights must be in range[0.0, 1.0] and sum to 1.0");
    }
}

void Material::validateShininess(float shininess) {
    if (shininess <= 0.00f) {
        throw std::invalid_argument("shininess must be greater than zero");
    }
}


std::ostream& operator<<(std::ostream& os, const Material& material) {
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
