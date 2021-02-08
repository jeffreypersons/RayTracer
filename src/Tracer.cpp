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
    const int width  = static_cast<int>(frameBuffer.getWidth());
    const int height = static_cast<int>(frameBuffer.getHeight());
    const float invWidth  = 1.00f / width;
    const float invHeight = 1.00f / height;
    #pragma omp for schedule(dynamic)
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // shoot a ray from camera position corresponding to pixel's position in viewport
            // and the directions of the camera
            float u = (col + 0.5f) * invWidth;
            float v = (row + 0.5f) * invHeight;
            Ray primaryRay = renderCam.getRay(u, v);
            Color color = traceRay(renderCam, scene, primaryRay, 0);
            frameBuffer.setPixel(row, col, color);
        }
    }
}

Color Tracer::traceRay(const RenderCam& renderCam, const Scene& scene, const Ray& ray, size_t iteration=0) const {
    if (iteration >= maxNumReflections) {
        return maxReflectedColor;
    }

    // find our nearest intersection
    IntersectInfo hit{};
    float tClosest = std::numeric_limits<float>::infinity();
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        IntersectInfo h;
        if (scene.getObject(index).intersect(ray, h) && h.t < tClosest) {
            hit      = h;
            tClosest = h.t;
        }
    }
    if (hit.object == nullptr) {
        return backgroundColor;
    }

    // reflect our ray throughout the scene using a slight directional offset and iteration cap to avoid infinite reflections
    Color reflectedColor = backgroundColor;
    if (hit.object->getMaterial().getReflectivity() > 0.00f) {
        reflectedColor = traceRay(renderCam, scene, reflectRay(ray, hit), iteration + 1);
    }
    
    Color ambient = computeAmbientColor(hit, scene.getLight(0));
    if (isInShadow(hit, scene.getLight(0), renderCam, scene)) {
        return ambient;
    }

    Color diffuse  = computeDiffuseColor( hit, scene.getLight(0));
    Color specular = computeSpecularColor(hit, scene.getLight(0), renderCam);

    // blend intrinsic and reflected color using our light and intersected object
    Color nonReflectedColor = ambient + diffuse + specular;
    return hit.object->getMaterial().getReflectivity() * reflectedColor +
           hit.object->getMaterial().getIntrinsity()   * nonReflectedColor;
}

// reflect our ray using a slight direction offset to avoid infinite reflections
Ray Tracer::reflectRay(const Ray& ray, const IntersectInfo& hit) const {
    Vec3 reflectedVec = (-1.0f * ray.direction) - (2.0f * hit.normal) * (Math::dot(ray.direction, hit.normal));
    return Ray(hit.point + reflectionalScalar * hit.normal, Math::normalize(reflectedVec));
}

// check if there exists an object blocking light from reaching our hit-point
bool Tracer::isInShadow(const IntersectInfo& hit, const Light& light, const RenderCam& renderCam, const Scene& scene) const {
    Ray shadowRay(hit.point, Math::direction(hit.point, light.getPosition()));
    float tLight = Math::distance(light.getPosition(), hit.point) - minTForShadowIntersections;
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        IntersectInfo h;
        if (scene.getObject(index).intersect(shadowRay, h) && h.t > minTForShadowIntersections && h.t < tLight) {
            return true;
        }
    }
    return false;
}

// blend intrinsic and reflected color using our light and intersected object
Color Tracer::computeAmbientColor(const IntersectInfo& hit, const Light& light) const {
    return hit.object->getMaterial().getAmbientColor() * light.getAmbientColor();
}
// blend intrinsic and reflected color using our light and intersected object
Color Tracer::computeDiffuseColor(const IntersectInfo& hit, const Light& light) const {
    Vec3 directionToLight = Math::direction(hit.point, light.getPosition());
    return Math::dot(hit.normal, directionToLight) * hit.object->getMaterial().getDiffuseColor() * light.getDiffuseColor();
}
// blend intrinsic and reflected color using our light and intersected object
Color Tracer::computeSpecularColor(const IntersectInfo& hit, const Light& light, const RenderCam& renderCam) const {
    Vec3 directionToCam = Math::direction(hit.point, renderCam.getPosition());
    Vec3 halfwayVec = Math::normalize(directionToCam + light.getPosition());
    float intensity = Math::dot(hit.normal, halfwayVec);
    return intensity * hit.object->getMaterial().getSpecularColor() * light.getSpecularColor();
}
