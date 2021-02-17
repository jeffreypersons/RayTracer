#pragma once
#include "Math.hpp"
#include "Color.hpp"
#include "Cameras.hpp"
#include "Lights.hpp"
#include "Scene.hpp"
#include "SceneObjects.h"
#include "FrameBuffers.hpp"


class Tracer {
private:
    Color shadowColor;
    Color backgroundColor;
    float tracingBias;
    size_t maxNumReflections;

    static constexpr Color DEFAULT_SHADOW_COLOR     { 0.125f, 0.125f, 0.125f };
    static constexpr Color DEFAULT_BACKGROUND_COLOR { 0.500f, 0.500f, 0.500f };
    
    static constexpr float DEFAULT_TRACING_BIAS = 1e-02f;
    static constexpr size_t DEFAULT_MAX_NUM_REFLECTIONS = 3;

public:
    Tracer();
    void trace(const RenderCam&, const Scene&, FrameBuffer&);
    void setShadowColor(const Color&);
    void setBackgroundColor(const Color&);
    void setTracingBias(float);
    void setMaxNumReflections(size_t);

    Color getShadowColor()        const { return shadowColor;       }
    Color getBackgroundColor()    const { return backgroundColor;   }
    float getTracingBias()        const { return tracingBias;       }
    size_t getMaxNumReflections() const { return maxNumReflections; }

private:
    Color traceRay(const RenderCam&, const Scene&, const Ray&, size_t) const;

    Ray reflectRay(const Ray&, const IntersectInfo&) const;
    bool isInShadow(const IntersectInfo&, const PointLight&, const Scene&) const;
    
    bool findNearestIntersection(const Scene&, const Ray&, IntersectInfo&) const;
    Color computeDiffuseColor (const IntersectInfo&, const PointLight&) const;
    Color computeSpecularColor(const IntersectInfo&, const PointLight&, const RenderCam&) const;
};

inline std::ostream& operator<<(std::ostream& os, const Tracer& tracer) {
    os << "Tracer("
         << "shadow-color:("       << tracer.getShadowColor()       << "), "
         << "background-color:("   << tracer.getBackgroundColor()   << "), "
         << "tracing-bias:"        << tracer.getTracingBias()       << ", "
         << "max-num-reflections:" << tracer.getMaxNumReflections()
       << ")";
    return os;
}
