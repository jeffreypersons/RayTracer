#include "Tracer.h"
#include "Math.hpp"
#include "Colors.hpp"
#include "Cameras.hpp"
#include "Lights.hpp"
#include "SceneObjects.h"
#include "Scene.hpp"
#include "FrameBuffers.hpp"
#include <omp.h>


Tracer::Tracer() :
    maxNumReflections(DEFAULT_MAX_NUM_REFLECTIONS),
    backgroundColor(DEFAULT_BACKROUND_COLOR)
{}
Tracer::Tracer(size_t maxNumReflections, const Color& backgroundColor) :
    maxNumReflections(maxNumReflections),
    backgroundColor(backgroundColor)
{}

void Tracer::setMaxNumReflections(size_t maxNumReflections) {
    this->maxNumReflections = maxNumReflections;
}
void Tracer::setBackgroundColor(const Color& backgroundColor) {
    this->backgroundColor = backgroundColor;
}

// for each pixel in buffer trace ray from given camera through given scene,
// and write computed color to buffer (in parallel)
void Tracer::trace(const RenderCam& renderCam, const Scene& scene, FrameBuffer& frameBuffer) {
    // == trace ray for each pixel and set it to computed color
    // note that we parallelize using dynamic schedule since the time for tracing at each pixel can vary
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < frameBuffer.getWidth(); i++) {
        for (int j = 0; j < frameBuffer.getHeight(); j++) {
            // shoot a ray from camera position corresponding to pixel's position in viewport
            // and the directions of the camera
            float u = (i + 0.5f) / frameBuffer.getWidth();
            float v = (j + 0.5f) / frameBuffer.getHeight();
            Ray primaryRay = renderCam.getRay(u, v);
            Color color = traceRay(renderCam, scene, primaryRay, 0);
            frameBuffer.setColor(i, j, color);
        }
    }
}

Color Tracer::traceRay(const RenderCam& renderCam, const Scene& scene, const Ray& ray, size_t iteration=0) const {
    // find our nearest intersection
    float tClosest = std::numeric_limits<float>::infinity();
    RayHitInfo hit;
    const ISceneObject* object = nullptr;
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

    // ==== compute reflected color
    Color reflectedColor(0, 0, 0);
    const Material& surfaceMaterial = object->getMaterial();
    Vec3 V = Math::direction(hit.point, renderCam.getPosition());
    if (iteration < maxNumReflections && surfaceMaterial.reflectivity > 0.00f) {
        const double ERR = 1e-05;
        Vec3 reflectedVec = (-1.0f * V) + (2.0f * hit.normal) * (Math::dot(V, hit.normal));
        Ray reflectionRay(hit.point + hit.normal * ERR, Math::normalize(reflectedVec));
        reflectedColor = traceRay(renderCam, scene, reflectionRay, iteration + 1);
    }

    // extract light related data
    Light light = scene.getLight(0);
    Vec3 directionToLight = Math::direction(light.getPosition(), hit.point);

    // ==== compute shadow
    // find our nearest intersection
    // object casts a shadow (occludes P) IFF it intersects ray AND 0.0001 < t < distance(P, L)
    Ray shadowRay(hit.point, directionToLight);
    float tLight = Math::distance(hit.point, light.getPosition());
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        RayHitInfo h;
        if (scene.getObject(index).intersect(shadowRay, h)) {
            if (h.t > 0.001 && h.t < tLight) {
                return Color(0, 0, 0);
            }
        }
    }

    // ==== compute surface color
    Color surfaceColor;
    Vec3 H = Math::normalize(V + light.getPosition());
    Color ambientColor  = surfaceMaterial.ambient * light.getMaterial().ambient * light.getAmbientIntensity();
    Color diffuseColor  = Math::dot(hit.normal, directionToLight) *
        surfaceMaterial.diffuse * light.getMaterial().diffuse * light.getDiffuseIntensity();
    Color specularColor = Math::dot(hit.normal, H) *
        surfaceMaterial.specular * light.getMaterial().specular * light.getSpecularIntensity();
    surfaceColor = ambientColor + diffuseColor + specularColor;

    return surfaceMaterial.reflectivity * reflectedColor +
           surfaceMaterial.intrinsic    * surfaceColor;
}
