#pragma once
#include "Math.hpp"
#include "Colors.hpp"
#include "Cameras.hpp"
#include "Lights.hpp"
#include "Scene.hpp"
#include "SceneObjects.h"
#include "FrameBuffers.hpp"


namespace Presets {
const Material smoothBlue = Material(
    Color(0.00f, 0.00f, 0.75f),
    Color(0.00f, 0.00f, 0.75f),
    Color(0.00f, 0.00f, 0.50f),
    0.50f, 0.50f
);
const Material shinyBlue = Material(
    Color(0.00f, 0.00f, 0.25f),
    Color(0.00f, 0.00f, 0.15f),
    Color(0.00f, 0.00f, 1.00f),
    0.95f, 0.05f
);
const Material roughRed = Material(
    Color(0.50f, 0.01f, 0.05f),
    Color(0.50f, 0.10f, 0.05f),
    Color(1.00f, 0.01f, 0.05f),
    0.75f, 0.25f
);
const Material reflectiveGreen = Material(
    Color(0.25f, 0.10f, 0.25f),
    Color(0.25f, 1.00f, 0.25f),
    Color(0.25f, 0.50f, 0.25f),
    0.10f, 0.90f
);
const Material pureWhite = Material(
    Color(0.00f, 1.00f, 1.00f),
    Color(1.00f, 0.50f, 1.00f),
    Color(1.00f, 1.00f, 0.50f),
    1.00f, 0.00f
);
const Material flatYellow = Material(
    Color(1.00f, 1.00f, 0.00f),
    Color(1.00f, 1.00f, 0.00f),
    Color(1.00f, 1.00f, 0.00f),
    1.00f, 0.00f
);
};

class Tracer {
private:
    Color shadowColor;
    Color backgroundColor;
    size_t maxNumReflections;
    
    static constexpr Color DEFAULT_SHADOW_COLOR{ 0, 0, 0 };
    static constexpr Color DEFAULT_BACKGROUND_COLOR{ 0, 0, 0 };
    static constexpr size_t DEFAULT_MAX_NUM_REFLECTIONS = 5;
    Color traceRay(const RenderCam&, const Scene&, const Ray&, size_t) const;
    
public:
    Tracer();
    void trace(const RenderCam&, const Scene&, FrameBuffer&);
    void setShadowColor(const Color&);
    void setBackgroundColor(const Color&);
    void setMaxNumReflections(size_t);
};
