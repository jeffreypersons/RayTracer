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
    float minTForShadowIntersections;
    float reflectionalScalar;

    static constexpr Color DEFAULT_SHADOW_COLOR        { 0.125f, 0.125f, 0.125f };
    static constexpr Color DEFAULT_BACKGROUND_COLOR    { 0.000f, 0.000f, 0.000f };
    static constexpr Color DEFAULT_MAX_REFLECTED_COLOR { 1.000f, 0.750f, 0.750f };

    static constexpr size_t DEFAULT_MAX_NUM_REFLECTIONS           = 3;
    static constexpr float DEFAULT_MIN_T_FOR_SHADOW_INTERSECTIONS = 0.01000f;
    static constexpr float DEFAULT_REFLECTIONAL_SCALAR            = 0.00001f;
    
public:
    Tracer();
    void trace(const RenderCam&, const Scene&, FrameBuffer&);
    void setShadowColor(const Color&);
    void setBackgroundColor(const Color&);
    void setMaximumallyReflectedColor(const Color&);
    void setMaxNumReflections(size_t);
    void setMinTForShadowIntersections(float);
    void setReflectionalScalar(float);

private:
    Color traceRay(const RenderCam&, const Scene&, const Ray&, size_t) const;

    Ray reflectRay(const Ray&, const RayHitInfo&) const;
    bool isInShadow(const RenderCam&, const RayHitInfo&, const Scene&, const Light&) const;

    Color computeAmbientColor (const ISceneObject&, const Light&) const;
    Color computeDiffuseColor (const ISceneObject&, const Light&, const RayHitInfo&) const;
    Color computeSpecularColor(const ISceneObject&, const Light&, const RayHitInfo&, const RenderCam&) const;
};
