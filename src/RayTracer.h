#pragma once
#include "Math.hpp"
#include "Ray.hpp"
#include "Color.hpp"
#include "Camera.h"
#include "Lights.h"
#include "Objects.h"
#include "Scene.h"
#include "FrameBuffer.hpp"


struct RayTracerOptions {
    Color shadowColor;
    Color backgroundColor;
    float bias;
    size_t maxNumReflections;
    RayTracerOptions() = default;
};

class RayTracer {
private:
    const RayTracerOptions options_;
    
public:
    RayTracer(const RayTracerOptions& options);
    void traceScene(const Camera& camera, const Scene& scene, FrameBuffer& frameBuffer) const;
    RayTracerOptions options() const;

private:
    Color traceRay(const Camera& camera, const Scene& scene, const Ray& ray, size_t depth) const;

    Ray reflectRay(const Ray& ray, const Intersection& intersection) const;
    bool findNearestIntersection(const Camera& camera, const Scene& scene, const Ray& ray, Intersection& result) const;
    bool isInShadow(const Camera& camera, const Intersection& intersection, const ILight& light, const Scene& scene) const;

    Color computeDiffuseColor (const Intersection& intersection, const ILight& light) const;
    Color computeSpecularColor(const Intersection& intersection, const ILight& light, const Camera& camera) const;
};
std::ostream& operator<<(std::ostream& os, const RayTracer& tracer);
