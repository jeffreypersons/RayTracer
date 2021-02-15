#include "Tracer.h"
#include "Math.hpp"
#include "Color.hpp"
#include "Cameras.hpp"
#include "Lights.hpp"
#include "SceneObjects.h"
#include "Scene.hpp"
#include "FrameBuffers.hpp"
#include <omp.h>


Tracer::Tracer() :
    shadowColor      (DEFAULT_SHADOW_COLOR),
    backgroundColor  (DEFAULT_BACKGROUND_COLOR),
    tracingBias      (DEFAULT_TRACING_BIAS),
    maxNumReflections(DEFAULT_MAX_NUM_REFLECTIONS)
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
void Tracer::setTracingBias(float tracingBias) {
    this->tracingBias = tracingBias;
}

// for each pixel in buffer shoot ray from camera position to its projected point on the image plane,
// trace it through the scene and write computed color to buffer (dynamically scheduled in parallel using openMp)
void Tracer::trace(const RenderCam& renderCam, const Scene& scene, FrameBuffer& frameBuffer) {
    const int width        = static_cast<int>(frameBuffer.getWidth());
    const int height       = static_cast<int>(frameBuffer.getHeight());
    const float invWidth   = 1.00f / width;
    const float invHeight  = 1.00f / height;
    const float nearZ      = renderCam.getNearClip();
    const Vec3 eyePosition = renderCam.getPosition();
    #pragma omp for schedule(dynamic)
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            const Vec3 pixelWorldPosition = renderCam.viewportToWorld(Vec3(
                (col + 0.50f) * invWidth,
                (row + 0.50f) * invHeight,
                nearZ
            ));
            Ray primaryRay{ eyePosition, Math::direction(eyePosition, pixelWorldPosition) };
            Color pixelColor = traceRay(renderCam, scene, primaryRay, 0);
            frameBuffer.setPixel(row, col, pixelColor);
        }
    }
}

// todo: account for near and far clip culling (probably need to determine z dist pf object to camera and clamp on that)
Color Tracer::traceRay(const RenderCam& renderCam, const Scene& scene, const Ray& ray, size_t iteration=0) const {
    if (iteration >= maxNumReflections) {
        return backgroundColor;
    }

    IntersectInfo intersection{};
    if (!findNearestIntersection(scene, ray, intersection)) {
        return backgroundColor;
    }

    Color reflectedColor = backgroundColor;
    if (intersection.object->getMaterial().getReflectivity() > 0.00f) {
        reflectedColor = traceRay(renderCam, scene, reflectRay(ray, intersection), iteration + 1);
    }
    
    PointLight light = scene.getLight(0);
    Color ambient = intersection.object->getMaterial().getAmbientColor();
    if (isInShadow(intersection, light, scene)) {
        return shadowColor;
    }

    Color lightIntensityAtPoint = light.computeIntensityAtPoint(intersection.point);
    Color diffuse  = computeDiffuseColor(intersection, light);
    Color specular = computeSpecularColor(intersection, light, renderCam);

    // blend intrinsic and reflected color using our light and intersected object
    Color nonReflectedColor = ambient + lightIntensityAtPoint * (diffuse + specular);
    return intersection.object->getMaterial().getReflectivity() * reflectedColor +
           intersection.object->getMaterial().getIntrinsity()   * nonReflectedColor;
}

// reflect our ray using a slight direction offset to avoid infinite reflections
Ray Tracer::reflectRay(const Ray& ray, const IntersectInfo& intersection) const {
    Vec3 reflectedDirection = Math::normalize(
        (-1 * ray.direction) - (2 * Math::dot(ray.direction, intersection.normal) * intersection.normal)
    );
    return Ray(intersection.point + (tracingBias * intersection.normal), reflectedDirection);
}
bool Tracer::findNearestIntersection(const Scene& scene, const Ray& ray, IntersectInfo& result) const {
    float tClosest = Math::INF;
    IntersectInfo closestIntersection{};
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        IntersectInfo intersection;
        if (scene.getObject(index).intersect(ray, intersection) && intersection.t < tClosest) {
            tClosest = intersection.t;
            closestIntersection = intersection;
        }
    }

    if (closestIntersection.object == nullptr) {
        return false;
    } else {
        result = closestIntersection;
        return true;
    }
}
// check if there exists another object blocking light from reaching our hit-point
bool Tracer::isInShadow(const IntersectInfo& intersection, const PointLight& light, const Scene& scene) const {
    Ray shadowRay{ intersection.point + (tracingBias * intersection.normal), Math::direction(intersection.point, light.getPosition()) };
    float distanceToLight = Math::distance(shadowRay.origin, light.getPosition());
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        IntersectInfo occlusion;
        if (scene.getObject(index).intersect(shadowRay, occlusion) &&
                occlusion.object != intersection.object &&
                Math::distance(occlusion.point, light.getPosition()) < distanceToLight) {
            return true;
        }
    }
    return false;
}

Color Tracer::computeDiffuseColor(const IntersectInfo& intersection, const PointLight& light) const {
    Vec3 directionToLight = Math::direction(intersection.point, light.getPosition());

    const Material surfaceMaterial = intersection.object->getMaterial();
    float strengthAtLightAngle = Math::max(0.00f, Math::dot(intersection.normal, directionToLight));
    return strengthAtLightAngle * surfaceMaterial.getDiffuseColor();
}

Color Tracer::computeSpecularColor(const IntersectInfo& intersection, const PointLight& light, const RenderCam& renderCam) const {
    Vec3 directionToCam = Math::direction(intersection.point, renderCam.getPosition());
    Vec3 halfwayVec = Math::normalize(directionToCam + light.getPosition());

    const Material surfaceMaterial = intersection.object->getMaterial();
    float strengthAtCamAngle = Math::max(0.00f, Math::dot(intersection.normal, halfwayVec));
    return Math::pow(strengthAtCamAngle, surfaceMaterial.getShininess()) * surfaceMaterial.getSpecularColor();
}
