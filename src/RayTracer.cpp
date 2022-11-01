#include "RayTracer.hpp"
#include "Math.hpp"
#include "Color.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "Objects.hpp"
#include "LightIncidence.hpp"
#include "Scene.hpp"
#include "FrameBuffer.hpp"
#include <omp.h>

RayTracer::RayTracer()
    : bias_             (DEFAULT_BIAS),
      maxIncidences_    (DEFAULT_MAX_NUM_INCIDENCES),
      shadowColor_      (DEFAULT_SHADOW_COLOR),
      backgroundColor_  (DEFAULT_BACKGROUND_COLOR) {}


// for each pixel in buffer shoot ray from camera position to its projected point on the image plane,
// traceScene it through the scene and write computed color to buffer (dynamically scheduled in parallel using openMp)
void RayTracer::traceScene(const Camera& camera, const Scene& scene, FrameBuffer& frameBuffer) const {
    // use ints for indexing since size_t is not supported by openMp loop parallelization macros
    const size_t width     = static_cast<int>(frameBuffer.width());
    const size_t height    = static_cast<int>(frameBuffer.height());
    const float invWidth   = 1.00f / width;
    const float invHeight  = 1.00f / height;
    const Vec3 eyePosition = camera.position();
    const int numPixels    = static_cast<int>(frameBuffer.numPixels());
#ifndef DEBUG
    #pragma omp for schedule(dynamic)
#else
    std::cout << "Not using OpenMP...";
#endif
    for (int i = 0; i < numPixels; i++) {
        const auto [row, col] = frameBuffer.getPixelRowCol(i);
        const Vec3 viewportPosition{ (col + 0.50f) * invWidth, (row + 0.50f) * invHeight, 0.00f };
        const Ray primaryRay = camera.viewportPointToRay(viewportPosition);
        const Color pixelColor = traceRay(camera, scene, nullptr, primaryRay, 0);
        frameBuffer.setPixel(height - 1 - row, col, pixelColor);  // invert y (since viewport and row start opposite)
    }
}

float RayTracer::bias() const {
    return bias_;
}

size_t RayTracer::maxNumIncidences() const {
    return maxIncidences_;
}

Color RayTracer::shadowColor() const {
    return shadowColor_;
}

Color RayTracer::backgroundColor() const {
    return backgroundColor_;
}


void RayTracer::setBias(float bias) {
    this->bias_ = bias;
}

void RayTracer::setMaxNumReflections(size_t maxNumReflections) {
    this->maxIncidences_ = maxNumReflections;
}

void RayTracer::setShadowColor(const Color& shadowColor) {
    this->shadowColor_ = shadowColor;
}

void RayTracer::setBackgroundColor(const Color& backgroundColor) {
    this->backgroundColor_ = backgroundColor;
}

Color RayTracer::traceRay(const Camera& camera, const Scene& scene, const IObject* inObject, const Ray& ray, size_t depth) const {
    Intersection intersection{};
    if (!findNearestIntersection(camera, scene, ray, intersection)) {
        return backgroundColor_;
    }

    const Material& object_material = intersection.object->material();

    Color blendedColor;
    Color intrinsicColor = scene.getAmbientLightColor();
    Color reflectedColor = backgroundColor_;
    Color refractedColor = backgroundColor_;
    float transmittedProportion = 1.0;
    float reflectedProportion = 0.0;

    Ray outgoingRay{};

    if (depth < maxIncidences_) {
        float n1, n2;
        n1 = (inObject != nullptr) ? inObject->material().indexOfRefraction() : 1.0f;
        n2 = object_material.indexOfRefraction();

        LightIncidenceFresnel incidence{ray, intersection, n1, n2};

        outgoingRay = Ray{intersection.point, incidence.reflectedDirection()};
        biasRayInDirection(outgoingRay, intersection.normal);
        reflectedColor = traceRay(camera, scene, inObject, outgoingRay, depth + 1);
        reflectedProportion = object_material.reflectivity() * incidence.reflectedProportion();

        if (reflectedProportion < 0.90) {
            outgoingRay = Ray{intersection.point, incidence.refractedDirection()};
            biasRayInDirection(outgoingRay, -intersection.normal);
            const IObject* nextInObject = (intersection.object == inObject) ? nullptr : intersection.object;
            refractedColor = traceRay(camera, scene, nextInObject, outgoingRay, depth + 1);
        }
    }

    // lights
    for (size_t index = 0; index < scene.getNumLights(); index++) {
        const ILight& light = scene.getLight(index);
        Color diffuse  = computeDiffuseColor(intersection, light);
        Color specular = computeSpecularColor(intersection, light, camera);
        Color lightIntensityAtPoint = light.computeIntensityAtPoint(intersection.point);
        intrinsicColor += lightIntensityAtPoint * (diffuse + specular);
    }

    // shadows
    for (size_t index = 0; index < scene.getNumLights(); index++) {
        const ILight& light = scene.getLight(index);
        if (isInShadow(camera, intersection, light, scene)) {
            intrinsicColor -= shadowColor_;
        }
    }

    transmittedProportion = 1.0f - reflectedProportion;
    const float& transparency = intersection.object->material().transparency();
    blendedColor = transmittedProportion * ((1.0f - transparency)*intrinsicColor + transparency*refractedColor) +
                   reflectedProportion * reflectedColor;


    return blendedColor;
}

void RayTracer::biasRayInDirection(Ray& ray, const Vec3& direction) const {
    ray.origin += bias_ * direction;
}

bool RayTracer::findNearestIntersection(const Camera& camera, const Scene& scene, const Ray& ray, Intersection& result) const {
    float tClosest = Math::INF;
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        Intersection intersection;
        if (scene.getObject(index).intersect(ray, intersection) && intersection.t < tClosest) {
            tClosest = intersection.t;
            result = intersection;
        }
    }
    return (result.object != nullptr);
}

// check if there exists another object blocking light from reaching our hit-point
bool RayTracer::isInShadow(const Camera& camera, const Intersection& intersection, const ILight& light, const Scene& scene) const {
    const Vec3 directionToLight = Math::direction(intersection.point, light.position());
    const Ray shadowRay{ intersection.point, directionToLight };
    const float distanceToLight = Math::distance(shadowRay.origin, light.position());
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        Intersection occlusion;
        bool intersected = scene.getObject(index).intersect(shadowRay, occlusion);
        if (intersected && occlusion.object != intersection.object &&
                Math::distance(occlusion.point, light.position()) < distanceToLight) {
            return true;
        }
    }
    return false;
}

Color RayTracer::computeDiffuseColor(const Intersection& intersection, const ILight& light) const {
    const Vec3 directionToLight      = Math::direction(intersection.point, light.position());
    const Material surfaceMaterial   = intersection.object->material();
    const float strengthAtLightAngle = Math::max(0.00f, Math::dot(intersection.normal, directionToLight));
    return strengthAtLightAngle * surfaceMaterial.diffuseColor();
}

Color RayTracer::computeSpecularColor(const Intersection& intersection, const ILight& light, const Camera& camera) const {
    const Vec3 directionToCam      = Math::direction(intersection.point, camera.position());
    const Vec3 halfwayVec          = Math::normalize(directionToCam + light.position());
    const Material surfaceMaterial = intersection.object->material();
    const float strengthAtCamAngle = Math::max(0.00f, Math::dot(intersection.normal, halfwayVec));
    return Math::pow(strengthAtCamAngle, surfaceMaterial.shininess()) * light.intensity();
}


std::ostream& operator<<(std::ostream& os, const RayTracer& rayTracer) {
    os << "RayTracer("
         << "shadow-color:("       << rayTracer.shadowColor()       << "),"
         << "background-color:("   << rayTracer.backgroundColor()   << "),"
         << "bias:"                << rayTracer.bias()              << ","
         << "max-num-incidences:"  << rayTracer.maxNumIncidences()
       << ")";
    return os;
}
