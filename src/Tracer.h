#pragma once
#include "Math.hpp"
#include "Ray.hpp"
#include "Color.hpp"
#include "Camera.h"
#include "Lights.h"
#include "Objects.h"
#include "Scene.h"
#include "FrameBuffer.h"


class Tracer {
private:
    Color shadowColor_;
    Color backgroundColor_;
    float shadowBias_;
    float reflectionBias_;
    size_t maxNumReflections_;

    static constexpr Color DEFAULT_SHADOW_COLOR     { 0.125f, 0.125f, 0.125f };
    static constexpr Color DEFAULT_BACKGROUND_COLOR { 0.500f, 0.500f, 0.500f };
    
    static constexpr float DEFAULT_SHADOW_BIAS     = 1e-02f;
    static constexpr float DEFAULT_REFLECTION_BIAS = 1e-02f;
    static constexpr size_t DEFAULT_MAX_NUM_REFLECTIONS = 3;

public:
    Tracer();
    void trace(const Camera&, const Scene&, FrameBuffer&);

    void setShadowColor(const Color& shadowColor);
    void setBackgroundColor(const Color& backgroundColor);
    void setShadowBias(float shadowBias);
    void setReflectionBias(float reflectionBias);
    void setMaxNumReflections(size_t maxNumReflections);

    Color shadowColor()        const;
    Color backgroundColor()    const;
    float shadowBias()         const;
    float reflectionBias()     const;
    size_t maxNumReflections() const;

private:
    Color traceRay(const Camera& camera, const Scene& scene, const Ray& ray, size_t ) const;

    Ray reflectRay(const Ray& ray, const Intersection& intersection) const;
    bool isInShadow(const Intersection& intersection, const ILight& light, const Scene& scene) const;
    
    bool findNearestIntersection(const Scene& scene, const Ray& ray, Intersection& result) const;
    Color computeDiffuseColor (const Intersection& intersection, const ILight& light) const;
    Color computeSpecularColor(const Intersection& intersection, const ILight& light, const Camera& camera) const;
};
std::ostream& operator<<(std::ostream& os, const Tracer& tracer);
