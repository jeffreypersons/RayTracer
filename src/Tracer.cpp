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
    reflectionalBias(DEFAULT_REFLECTIONAL_BIAS),
    shadowBias(DEFAULT_SHADOW_BIAS)
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

// todo: account for farClip
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
    return Ray(hit.point + reflectionalBias * hit.normal, Math::normalize(reflectedVec));
}

// check if there exists an object blocking light from reaching our hit-point
bool Tracer::isInShadow(const IntersectInfo& hit, const Light& light, const RenderCam& renderCam, const Scene& scene) const {
    Ray shadowRay{ hit.point + (hit.normal * shadowBias), Math::direction(hit.point, light.getPosition()) };
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
    return hit.object->getMaterial().getAmbientColor() * light.getColor();
}
// blend intrinsic and reflected color using our light and intersected object
Color Tracer::computeDiffuseColor(const IntersectInfo& hit, const Light& light) const {
    Vec3 lightDirection;
    Vec3 lightIntensity;
    float distance;

    light.illuminate(hit.point, lightDirection, lightIntensity, distance);
    Vec3 result = Math::max(0.00f, Math::dot(hit.normal, -1 * lightDirection)) *
        Vec3(hit.object->getMaterial().getDiffuseColor().r * lightIntensity.x,
             hit.object->getMaterial().getDiffuseColor().g * lightIntensity.y,
             hit.object->getMaterial().getDiffuseColor().b * lightIntensity.z);
    return Color(result.x, result.y, result.z);
}

// blend intrinsic and reflected color using our light and intersected object
Color Tracer::computeSpecularColor(const IntersectInfo& hit, const Light& light, const RenderCam& renderCam) const {
    Vec3 directionToCam = Math::direction(hit.point, renderCam.getPosition());
    Vec3 halfwayVec = Math::normalize(directionToCam + light.getPosition());
    float intensity = Math::dot(hit.normal, halfwayVec);
    return intensity * hit.object->getMaterial().getSpecularColor() * light.getColor();
}

