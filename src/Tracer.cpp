#include "Tracer.h"
#include "Math.hpp"
#include "Color.hpp"
#include "Cameras.hpp"
#include "Lights.hpp"
#include "SceneObjects.h"
#include "Scene.hpp"
#include "FrameBuffers.hpp"
#include <omp.h>


// todo: consider adding object.contains() call to warn about camera position if it lies within
//       an object in the scene (thus creating strange results)
Tracer::Tracer() :
    maxNumReflections(DEFAULT_MAX_NUM_REFLECTIONS),
    shadowColor(DEFAULT_SHADOW_COLOR),
    backgroundColor(DEFAULT_BACKGROUND_COLOR),
    maxReflectedColor(DEFAULT_MAX_REFLECTED_COLOR),
    minTForShadowIntersections(DEFAULT_MIN_T_FOR_SHADOW_INTERSECTIONS),
    reflectionalScalar(DEFAULT_REFLECTIONAL_SCALAR)
{}

void Tracer::setMaxNumReflections(size_t maxNumReflections) {
    this->maxNumReflections = maxNumReflections;
}
void Tracer::setShadowColor(const Color& shadowColor) {
    this->shadowColor = shadowColor;
}
void Tracer::setBackgroundColor(const Color& backgroundColor) {
    this->backgroundColor = backgroundColor;
}
void Tracer::setMaximumallyReflectedColor(const Color& maxReflectedColor) {
    this->maxReflectedColor = maxReflectedColor;
}
void Tracer::setMinTForShadowIntersections(float minTForShadowIntersections) {
    this->minTForShadowIntersections = minTForShadowIntersections;
}
void Tracer::setReflectionalScalar(float reflectionalScalar) {
    this->reflectionalScalar = reflectionalScalar;
}

// for each pixel in buffer trace ray from given camera through given scene,
// and write computed color to buffer (dynamically scheduled in parallel using openMp)
void Tracer::trace(const RenderCam& renderCam, const Scene& scene, FrameBuffer& frameBuffer) {
    #pragma omp for schedule(dynamic)
    for (int row = 0; row < frameBuffer.getHeight(); row++) {
        for (int col = 0; col < frameBuffer.getWidth(); col++) {
            // shoot a ray from camera position corresponding to pixel's position in viewport
            // and the directions of the camera
            float u = (col + 0.5f) / frameBuffer.getWidth();
            float v = (row + 0.5f) / frameBuffer.getHeight();
            Ray primaryRay = renderCam.getRay(u, v);
            Color color = traceRay(renderCam, scene, primaryRay, 0);
            frameBuffer.setColor(row, col, color);
        }
    }
}

Color Tracer::traceRay(const RenderCam& renderCam, const Scene& scene, const Ray& ray, size_t iteration=0) const {
    if (iteration >= maxNumReflections) {
        return maxReflectedColor;
    }

    // find our nearest intersection
    RayHitInfo hit;
    const ISceneObject* object = nullptr;
    float tClosest = std::numeric_limits<float>::infinity();
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        RayHitInfo h;
        if (scene.getObject(index).intersect(ray, h) && h.t < tClosest) {
            hit      = h;
            tClosest = h.t;
            object   = &scene.getObject(index);
        }
    }
    if (object == nullptr) {
        return backgroundColor;
    }

    // reflect our ray throughout the scene using a slight directional offset
    // and iteration cap to avoid infinite reflections
    Color reflectedColor(0, 0, 0);
    const Material& surfaceMaterial = object->getMaterial();
    if (surfaceMaterial.reflectivity > 0.00f) {
        Vec3 reflectedVec = (-1.0f * ray.direction) + (2.0f * hit.normal) * (Math::dot(ray.direction, hit.normal));
        Ray reflectionRay(hit.point + reflectionalScalar * hit.normal, Math::normalize(reflectedVec));
        reflectedColor = traceRay(renderCam, scene, reflectionRay, iteration + 1);
    }

    // extract light related data
    Light light = scene.getLight(0);
    Vec3 directionToLight = Math::direction(light.getPosition(), hit.point);

    // check if there exists an object blocking light from reaching our hit-point
    Ray shadowRay(hit.point, directionToLight);
    float tLight = Math::distance(hit.point, light.getPosition()) - minTForShadowIntersections;
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        RayHitInfo h;
        if (scene.getObject(index).intersect(shadowRay, h) && h.t > minTForShadowIntersections && h.t < tLight) {
            return shadowColor;
        }
    }

    // blend intrinsic and reflected color using our light and intersected object
    Color surfaceColor;
    Vec3 directionToCam = Math::direction(hit.point, renderCam.getPosition());
    Vec3 halfwayVec = Math::normalize(directionToCam + light.getPosition());
    Color ambientColor  = surfaceMaterial.ambient * light.getMaterial().ambient * light.getAmbientIntensity();
    Color diffuseColor  = Math::dot(hit.normal, directionToLight) *
        surfaceMaterial.diffuse * light.getMaterial().diffuse * light.getDiffuseIntensity();
    Color specularColor = Math::dot(hit.normal, halfwayVec) *
        surfaceMaterial.specular * light.getMaterial().specular * light.getSpecularIntensity();
    surfaceColor = ambientColor + diffuseColor + specularColor;

    return surfaceMaterial.reflectivity * reflectedColor +
           surfaceMaterial.intrinsic    * surfaceColor;
}
