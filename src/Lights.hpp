#pragma once
#include "Math.hpp"
#include "Color.hpp"


class Light {
private:
    Vec3 position;
    Material material;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;

public:
    Light(Vec3 position, Material material) :
        position(position),
        material(material),
        ambientIntensity(1.00f),
        diffuseIntensity(1.00f),
        specularIntensity(1.00f) {}

    void setAmbientIntensity(float ambientIntensity)   { this->ambientIntensity = ambientIntensity;   }
    void setDiffuseIntensity(float diffuseIntensity)   { this->diffuseIntensity = diffuseIntensity;   }
    void setSpecularIntensity(float specularIntensity) { this->specularIntensity = specularIntensity; }
    float getAmbientIntensity()   const { return ambientIntensity; }
    float getDiffuseIntensity()   const { return diffuseIntensity; }
    float getSpecularIntensity()  const { return specularIntensity; }
    const Vec3& getPosition()     const { return position; }
    const Material& getMaterial() const { return material; }
};
