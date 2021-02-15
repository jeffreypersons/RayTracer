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
    maxNumReflections(DEFAULT_MAX_NUM_REFLECTIONS),
    shadowColor      (DEFAULT_SHADOW_COLOR),
    backgroundColor  (DEFAULT_BACKGROUND_COLOR),
    reflectionalBias (DEFAULT_REFLECTIONAL_BIAS),
    shadowBias       (DEFAULT_SHADOW_BIAS)
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
void Tracer::setReflectionalBias(float reflectionalBias) {
    this->reflectionalBias = reflectionalBias;
}
void Tracer::setShadowBias(float shadowBias) {
    this->shadowBias = shadowBias;
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
        return Color(0, 0, 0);
    }

    IntersectInfo hit{};
    if (!findNearestIntersection(scene, ray, hit)) {
        return backgroundColor;
    }

    // reflect our ray throughout the scene using a slight directional offset and iteration cap to avoid infinite reflections
    Color reflectedColor = backgroundColor;
    if (hit.object->getMaterial().getReflectivity() > 0.00f) {
        reflectedColor = traceRay(renderCam, scene, reflectRay(ray, hit), iteration + 1);
    }
    
    PointLight light = scene.getLight(0);
    Color ambient = hit.object->getMaterial().getAmbientColor();
    if (isInShadow(hit, light, scene)) {
        return ambient;
    }

    Color lightIntensityAtPoint = light.computeIntensityAtPoint(hit.point);
    Color diffuse  = computeDiffuseColor(hit, light);
    Color specular = computeSpecularColor(hit, light, renderCam);

    // blend intrinsic and reflected color using our light and intersected object
    Color nonReflectedColor = ambient + lightIntensityAtPoint * (diffuse + specular);
    return hit.object->getMaterial().getReflectivity() * reflectedColor +
           hit.object->getMaterial().getIntrinsity()   * nonReflectedColor;
}

// reflect our ray using a slight direction offset to avoid infinite reflections
Ray Tracer::reflectRay(const Ray& ray, const IntersectInfo& hit) const {
    Vec3 reflectedVec = (-1.0f * ray.direction) - (2.0f * hit.normal) * (Math::dot(ray.direction, hit.normal));
    return Ray(hit.point + (reflectionalBias * hit.normal), Math::normalize(reflectedVec));
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
// check if there exists an object blocking light from reaching our hit-point
bool Tracer::isInShadow(const IntersectInfo& hit, const PointLight& light, const Scene& scene) const {
    Ray shadowRay{ hit.point + (shadowBias * hit.normal), Math::direction(hit.point, light.getPosition()) };
    float distanceToLight = Math::distance(shadowRay.origin, light.getPosition());
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        IntersectInfo h;
        if (scene.getObject(index).intersect(shadowRay, h) &&
                (Math::distance(h.point, light.getPosition()) < distanceToLight)) {
            return true;
        }
    }
    return false;
}

Color Tracer::computeDiffuseColor(const IntersectInfo& hit, const PointLight& light) const {
    Vec3 directionToLight = Math::direction(hit.point, light.getPosition());
    float strengthAtAngle = Math::max(0.00f, Math::dot(hit.normal, directionToLight));
    return strengthAtAngle * hit.object->getMaterial().getDiffuseColor();
}

Color Tracer::computeSpecularColor(const IntersectInfo& hit, const PointLight& light, const RenderCam& renderCam) const {
    Vec3 directionToCam = Math::direction(hit.point, renderCam.getPosition());
    Vec3 halfwayVec = Math::normalize(directionToCam + light.getPosition());
    float strengthAtAngle = Math::max(0.00f, Math::dot(hit.normal, halfwayVec));
    return Math::pow(strengthAtAngle, hit.object->getMaterial().getShininess()) * hit.object->getMaterial().getSpecularColor();
}
