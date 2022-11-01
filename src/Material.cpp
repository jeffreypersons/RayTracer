#include "Material.hpp"
#include "Math.hpp"
#include "Color.hpp"
#include <iostream>


Material::Material()
    : Material(
        DEFAULT_DIFFUSE_COLOR,
        DEFAULT_REFLECTIVITY,
        DEFAULT_SPECULAR_EXPONENT,
        DEFAULT_TRANSPARENCY,
        DEFAULT_INDEX_OF_REFRACTION) {}

Material::Material(const Color& diffuseColor)
    : Material(
        diffuseColor,
        DEFAULT_REFLECTIVITY,
        DEFAULT_SPECULAR_EXPONENT,
        DEFAULT_TRANSPARENCY,
        DEFAULT_INDEX_OF_REFRACTION) {}

Material::Material(const Color& diffuseColor, float reflectivity, float shininess, float transparency, float indexOfRefraction)
    : diffuseColor_      (diffuseColor),
      reflectivity_      (reflectivity),
      shininess_         (shininess),
      transparency_      (transparency),
      indexOfRefraction_ (indexOfRefraction) {
    validateReflectivity(reflectivity_);
    validateShininess(shininess_);
    validateTransparency(transparency_);
    validateIndexOfRefraction(indexOfRefraction_);
}


Color Material::diffuseColor() const {
    return diffuseColor_;
}

float Material::reflectivity() const {
    return reflectivity_;
}

float Material::shininess() const {
    return shininess_;
}

float Material::transparency() const {
    return transparency_;
}

float Material::indexOfRefraction() const {
    return indexOfRefraction_;
}

void Material::setColor(const Color& diffuseColor) {
    this->diffuseColor_ = diffuseColor;
}

void Material::setReflectivity(float reflectivity) {
    validateReflectivity(reflectivity);
    this->reflectivity_ = reflectivity;
}

void Material::setShininess(float shininess) {
    validateShininess(shininess);
    this->shininess_ = shininess;
}

void Material::setTransparency(float transparency) {
    this->transparency_ = transparency;
}

void Material::setIndexOfRefraction(float indexOfRefraction) {
    validateIndexOfRefraction(indexOfRefraction);
    this->indexOfRefraction_ = indexOfRefraction;
}

void Material::validateReflectivity(float reflectivity) {
    if (reflectivity < 0.00f || reflectivity > 1.00f) {
        throw std::invalid_argument("reflectivity must be in range [0.0, 1.0]");
    }
}

void Material::validateShininess(float shininess) {
    if (shininess <= 0.00f) {
        throw std::invalid_argument("shininess must be greater than zero");
    }
}

void Material::validateTransparency(float transparency) {
    if (transparency < 0.00f || transparency > 1.00f) {
        throw std::invalid_argument("transparency must be in range [0.0, 1.0]");
    }
}

void Material::validateIndexOfRefraction(float indexOfRefraction) {
    if (indexOfRefraction < 1.00f) {
        throw std::invalid_argument("index of refraction must be at least 1.0");
    }
}

std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << "Material("
         << "Color: " << material.diffuseColor() << ", "
         << "Properies{"
           << "reflective:" << material.reflectivity() << ","
           << "transparency:" << material.transparency() << ","
           << "index-of-refraction:" << material.indexOfRefraction() << ","
         << "}, "
         << "Shininess{"
           << "specular-exponent:" << material.shininess()
         << "}, "
       << ")";
    return os;
}

namespace MaterialSample {
    const Material matteBlack      {Palette::black, 0.0f, 1.0f, 0.0f, 1.0f};
    const Material matteGray       {Palette::darkSlateGray, 0.0f, 1.0f, 0.0f, 1.0f};
    const Material reflectiveRed   {Palette::red, 0.5f, 15.0f, 0.0f, 3.0f};
    const Material reflectiveBlue  {Palette::blue, 0.5f, 15.0f, 0.0f, 3.0f};
    const Material reflectiveGreen {Palette::green, 0.5f, 15.0f, 0.0f, 3.0f};
    const Material transparentRed   {Palette::red, 0.5f, 15.0f, 0.8f, 1.2f};
    const Material transparentGreen {Palette::green, 0.5f, 15.0f, 0.8f, 1.2f};
    const Material transparentBlue  {Palette::blue, 0.5f, 15.0f, 0.8f, 1.2f};
}

