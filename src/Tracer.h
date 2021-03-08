#pragma once
#include "Math.hpp"
#include "Ray.hpp"
#include "Color.hpp"
#include "Camera.h"
#include "Lights.h"
#include "Objects.h"
#include "Scene.h"
#include "FrameBuffer.hpp"


class Tracer {
private:
    Color shadowColor_;
    Color backgroundColor_;
    float bias_;
    size_t maxNumReflections_;

    static constexpr Color DEFAULT_SHADOW_COLOR     { 0.125f, 0.125f, 0.125f };
    static constexpr Color DEFAULT_BACKGROUND_COLOR { 0.500f, 0.500f, 0.500f };
    static constexpr float DEFAULT_BIAS = 1e-02f;
    static constexpr size_t DEFAULT_MAX_NUM_REFLECTIONS = 3;

public:
    Tracer();
    void traceScene(const Camera& camera, const Scene& scene, FrameBuffer& frameBuffer) const;

    void setShadowColor(const Color& shadowColor);
    void setBackgroundColor(const Color& backgroundColor);
    void setBias(float bias);
    void setMaxNumReflections(size_t maxNumReflections);

    Color shadowColor()        const;
    Color backgroundColor()    const;
    float bias()               const;
    size_t maxNumReflections() const;

private:
    Color traceRay(const Camera& camera, const Scene& scene, const Ray& ray, size_t depth) const;

    Ray reflectRay(const Ray& ray, const Intersection& intersection) const;
    bool findNearestIntersection(const Camera& camera, const Scene& scene, const Ray& ray, Intersection& result) const;
    bool isInShadow(const Camera& camera, const Intersection& intersection, const ILight& light, const Scene& scene) const;

    Color computeDiffuseColor (const Intersection& intersection, const ILight& light) const;
    Color computeSpecularColor(const Intersection& intersection, const ILight& light, const Camera& camera) const;
};
std::ostream& operator<<(std::ostream& os, const Tracer& tracer);
