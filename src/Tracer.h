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
    Color maxReflectedColor;
    size_t maxNumReflections;
    
    static constexpr Color DEFAULT_SHADOW_COLOR       { 0.00f, 0.00f, 0.00f };
    static constexpr Color DEFAULT_BACKGROUND_COLOR   { 0.00f, 0.00f, 0.00f };
    static constexpr Color DEFAULT_MAX_REFLECTED_COLOR{ 0.05f, 0.05f, 0.05f };
    static constexpr size_t DEFAULT_MAX_NUM_REFLECTIONS = 5;
    static constexpr float T_OFFSET_FROM_POINT = 0.001f;
    Color traceRay(const RenderCam&, const Scene&, const Ray&, size_t) const;
    
public:
    Tracer();
    void trace(const RenderCam&, const Scene&, FrameBuffer&);
    void setShadowColor(const Color&);
    void setBackgroundColor(const Color&);
    void setMaximumallyReflectedColor(const Color&);
    void setMaxNumReflections(size_t);
};
