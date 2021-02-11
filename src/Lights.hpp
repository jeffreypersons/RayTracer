#pragma once
#include "Math.hpp"


// represents a point light in 3d space
class Light {
private:
    Vec3 position;
    Color color;
    float intensity;
    
    static constexpr Vec3 DEFAULT_POSITION { 0, 0, 0 };
    static constexpr float DEFAULT_INTENSITY { 0.50f };
    static constexpr Color DEFAULT_COLOR { 0.85f, 0.85f, 0.85f };
    
public:
    Light(const Vec3& position, const Color& color, float intensity) :
        position(position),
        color(DEFAULT_COLOR),
        intensity(DEFAULT_INTENSITY) {}

    // shade given point
    void illuminate(const Vec3& point, Vec3& lightDir, Vec3& lightIntensity, float& distance) const {
        float distanceSquared = Math::magnitudeSquared(point - this->position);
        lightDir = Math::direction(this->position, point);
        distance = Math::square(distanceSquared);
        lightIntensity = Vec3(color.r, color.g, color.b) * (intensity / (4 * Math::PI * distanceSquared));
    }

    void setPosition(const Vec3& position) { this->position = position; }
    void setColor(const Color& color)      { this->color    = color;    }
    void setIntensity(float intensity) {
        if (intensity < 0.00f || intensity > 1.00f) {
            throw std::invalid_argument("light intensity must be in range [0.00, 1.00]");
        }
        this->intensity = intensity;
    }

    Vec3 getPosition()   const { return position;  }
    Color getColor()     const { return color;     }
    float getIntensity() const { return intensity; }
};
inline std::ostream& operator<<(std::ostream& os, const Light& light) {
    os << std::fixed << std::setprecision(2)
       << "Light("
         << "position:(" << light.getPosition() << "), "
         << "color:("    << light.getColor()    << "), "
         << "intensity:" << light.getColor()
       << ")";
    return os;
}
