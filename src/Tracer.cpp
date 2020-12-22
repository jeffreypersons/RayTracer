#include "Tracer.h"
#include "Math.hpp"
#include "Colors.hpp"
#include "Cameras.hpp"
#include "Lights.hpp"
#include "SceneObjects.hpp"
#include "FrameBuffers.hpp"
#include <omp.h>


Tracer::Tracer() :
    outputFile(),
    frameBuffer(),
    viewPlane(),
    renderCam(),
    sceneLights(),
    sceneObjects()
{}

void Tracer::setup() {
    // == configure application window properties
    // == configure framebuffer
    frameBuffer = std::make_unique<FrameBuffer>(IMAGE_SIZE.x, IMAGE_SIZE.y, BACKGROUND_COLOR);
    
    // == configure view plane in worldspace orthogonal to cam at distance z into the screen
    viewPlane = std::make_shared<ViewPlane>(10, 10, 10);
    renderCam = std::make_unique<RenderCam>(viewPlane);
    frameBuffer = std::make_unique<FrameBuffer>(IMAGE_SIZE.x, IMAGE_SIZE.y, BACKGROUND_COLOR);

    // == initialize scene lights and objects
    sceneLights.push_back(Light(Vec3(0, 0, 10), Presets::pureWhite));
    sceneLights[0].setAmbientIntensity(0.50f);
    sceneLights[0].setDiffuseIntensity(0.75f);
    sceneLights[0].setSpecularIntensity(0.50f);

    // green triangle directly in front of camera a bit in front and to the right of the blue sphere
    // blue sphere directly in front of camera
    // small red sphere a bit off to the side
    sceneObjects.push_back(std::make_unique<Triangle>(Vec3(-10, 5, 5), Vec3(10, 5, 5), Vec3(0, 0, -25), Presets::flatYellow));
    sceneObjects.push_back(std::move(std::make_unique<Sphere>(Vec3( 2, -2, -10), 2.00f, Presets::smoothBlue)));
    sceneObjects.push_back(std::move(std::make_unique<Sphere>(Vec3(-2,  5, -15), 1.25f, Presets::roughRed)));
    sceneObjects.push_back(std::move(std::make_unique<Sphere>(Vec3(-2, 10, -15), 0.50f, Presets::smoothBlue)));
    sceneObjects.push_back(std::move(std::make_unique<Sphere>(Vec3( 0,  5, -30), 1.00f, Presets::shinyBlue)));
    sceneObjects.push_back(std::move(std::make_unique<Sphere>(Vec3( 0,  5, -15), 1.00f, Presets::shinyBlue)));

    // == trace ray for each pixel and set it to computed color
    // note that we parallelize using dynamic schedule since the time for tracing at each pixel can vary
    std::cout << "Tracing scene...";
    double startTime = omp_get_wtime();
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < frameBuffer->getWidth(); i++) {
        for (int j = 0; j < frameBuffer->getHeight(); j++) {
            // shoot a ray from camera position corresponding to pixel's position in viewport
            // and the directions of the camera
            float u = (i + 0.5f) / frameBuffer->getWidth();
            float v = (j + 0.5f) / frameBuffer->getHeight();
            Ray primaryRay = renderCam->getRay(u, v);
            Color color = traceRay(primaryRay, 0);
            frameBuffer->setColor(i, j, color);
        }
    }
    double endTime = omp_get_wtime() - startTime;
    std::cout << "finished in " << endTime << "\n";
    
    // == write each pixel to screen
    std::cout << "Writing frameBuffer to image file: " << OUTPUT_FILE << ".ppm`" << "\n";
    frameBuffer->writeToFile(OUTPUT_FILE);
}

void Tracer::draw() const {
    // ??
}

Color Tracer::traceRay(const Ray& ray, size_t iteration=0) const {
    // find our nearest intersection
    float tClosest = std::numeric_limits<float>::infinity();
    RayHitInfo hit;
    ISceneObject* object = nullptr;
    for (const auto& sceneObject : sceneObjects) {
        RayHitInfo h;
        if (sceneObject->intersect(ray, h) && h.t < tClosest) {
            hit      = h;
            tClosest = h.t;
            object   = sceneObject.get();
        }
    }
    if (object == nullptr) {
        return BACKGROUND_COLOR;
    }

    // ==== compute reflected color
    Color reflectedColor(0, 0, 0);
    const Material& surfaceMaterial = object->getMaterial();
    Vec3 V = Math::direction(hit.point, renderCam->getPosition());
    if (iteration < MAX_NUM_REFLECTIONS && surfaceMaterial.reflectivity > 0.00f) {
        const double ERR = 1e-05;
        Vec3 reflectedVec = (-1.0f * V) + (2.0f * hit.normal) * (Math::dot(V, hit.normal));
        Ray reflectionRay(hit.point + hit.normal * ERR, Math::normalize(reflectedVec));
        reflectedColor = traceRay(reflectionRay, iteration + 1);
    }

    // extract light related data
    Light light = sceneLights[0];
    Vec3 directionToLight = Math::direction(light.getPosition(), hit.point);

    // ==== compute shadow
    // find our nearest intersection
    // object casts a shadow (occludes P) IFF it intersects ray AND 0.0001 < t < distance(P, L)
    Ray shadowRay(hit.point, directionToLight);
    float tLight = Math::distance(hit.point, light.getPosition());
    for (const auto& sceneObject : sceneObjects) {
        RayHitInfo h;
        if (sceneObject->intersect(shadowRay, h)) {
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
