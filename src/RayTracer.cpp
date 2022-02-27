#include "RayTracer.hpp"
#include "Math.hpp"
#include "Color.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "Objects.hpp"
#include "Scene.hpp"
#include "FrameBuffer.hpp"
#include <omp.h>


RayTracer::RayTracer()
    : bias_             (DEFAULT_BIAS),
      maxNumReflections_(DEFAULT_MAX_NUM_REFLECTIONS),
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
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < numPixels; i++) {
        const auto [row, col] = frameBuffer.getPixelRowCol(i);
        const Vec3 viewportPosition{ (col + 0.50f) * invWidth, (row + 0.50f) * invHeight, 0.00f };
        const Ray primaryRay = camera.viewportPointToRay(viewportPosition);
        const Color pixelColor = traceRay(camera, scene, primaryRay, 0);
        frameBuffer.setPixel(height - 1 - row, col, pixelColor);  // invert y (since viewport and row start opposite)
    }
}

float RayTracer::bias() const {
    return bias_;
}

size_t RayTracer::maxNumReflections() const {
    return maxNumReflections_;
}

Color RayTracer::shadowColor() const {
    return backgroundColor_;
}

Color RayTracer::backgroundColor() const {
    return backgroundColor_;
}


void RayTracer::setBias(float shadowBias) {
    this->bias_ = shadowBias;
}

void RayTracer::setMaxNumReflections(size_t maxNumReflections) {
    this->maxNumReflections_ = maxNumReflections;
}

void RayTracer::setShadowColor(const Color& shadowColor) {
    this->shadowColor_ = shadowColor;
}

void RayTracer::setBackgroundColor(const Color& backgroundColor) {
    this->backgroundColor_ = backgroundColor;
}



Color RayTracer::traceRay(const Camera& camera, const Scene& scene, const Ray& ray, size_t depth=0) const {
    Intersection intersection{};
    if (!findNearestIntersection(camera, scene, ray, intersection)) {
        return backgroundColor_;
    }

    Color reflectedColor{ 0.00, 0.00, 0.00 };
    if (depth < maxNumReflections_ && intersection.object->material().reflectivity() > 0.00f) {
        reflectedColor = traceRay(camera, scene, reflectRay(ray, intersection), depth + 1);
    }
    
    // todo: will need to adjust lighting model for weakening (or not summing) reflection values according to 'strength' of each shadow,
    //       and also a better way to include reflection from background (sky) without overpowering everything
    Color nonReflectedColor = intersection.object->material().ambientColor();
    for (size_t index = 0; index < scene.getNumLights(); index++) {
       const ILight& light = scene.getLight(index);
       //if (isInShadow(camera, intersection, light, scene)) { nonReflectedColor += shadowColor_; continue; }
       Color diffuse  = computeDiffuseColor(intersection, light);
       Color specular = computeSpecularColor(intersection, light, camera);
       Color lightIntensityAtPoint = light.computeIntensityAtPoint(intersection.point);
       nonReflectedColor += lightIntensityAtPoint * (diffuse + specular);
    }
    
    // blend intrinsic and reflected color using our light and intersected object
    return (intersection.object->material().intrinsity()   * nonReflectedColor) + 
           (intersection.object->material().reflectivity() * reflectedColor);
}

// reflect our ray using a slight direction offset to avoid infinite reflections
Ray RayTracer::reflectRay(const Ray& ray, const Intersection& intersection) const {
    const Vec3 reflectedDirection = Math::reflect(ray.direction, intersection.normal);
    return Ray(intersection.point + (bias_ * intersection.normal), reflectedDirection);
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
    const Ray shadowRay{
        intersection.point + (bias_ * intersection.normal),
        Math::direction(intersection.point, light.position())
    };
    const float distanceToLight = Math::distance(shadowRay.origin, light.position());
    for (size_t index = 0; index < scene.getNumObjects(); index++) {
        Intersection occlusion;
        if (scene.getObject(index).intersect(shadowRay, occlusion) &&
                occlusion.object != intersection.object &&
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
    return Math::pow(strengthAtCamAngle, surfaceMaterial.shininess()) * surfaceMaterial.specularColor();
}


std::ostream& operator<<(std::ostream& os, const RayTracer& rayTracer) {
    os << "RayTracer("
         << "shadow-color:("       << rayTracer.shadowColor()       << "),"
         << "background-color:("   << rayTracer.backgroundColor()   << "),"
         << "bias:"                << rayTracer.bias()              << ","
         << "max-num-reflections:" << rayTracer.maxNumReflections()
       << ")";
    return os;
}
