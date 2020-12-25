#include "Tracer.h"
#include "Scene.hpp"
#include "Timer.hpp"
#include "Presets.hpp"
#include <iostream>


const std::string OUTPUT_FILE = "./scene";
static constexpr Color SHADOW_COLOR{ 0.05f, 0.05f, 0.05f };
static constexpr Color BACKGROUND_COLOR{ 0.25f, 0.25f, 0.25f };
static constexpr size_t MAX_NUM_REFLECTIONS = 5;

Scene createSimpleScene() {
    Scene scene{};
    Light pointLight(Vec3(0, 0, 10), PresetMaterials::pureWhite);
    pointLight.setAmbientIntensity(0.50f);
    pointLight.setDiffuseIntensity(0.75f);
    pointLight.setSpecularIntensity(0.50f);
    scene.addLight(pointLight);
    scene.addSceneObject(Triangle(Vec3(-10, 5, 5), Vec3(10, 5, 5), Vec3(0, 0, -25), PresetMaterials::flatYellow));
    scene.addSceneObject(Sphere(Vec3(2, -2, -10), 2.00f, PresetMaterials::smoothBlue));
    scene.addSceneObject(Sphere(Vec3(-2, 5, -15), 1.25f, PresetMaterials::roughRed));
    scene.addSceneObject(Sphere(Vec3(-2, 10, -15), 0.50f, PresetMaterials::smoothBlue));
    scene.addSceneObject(Sphere(Vec3(0, 5, -30), 1.00f, PresetMaterials::shinyBlue));
    scene.addSceneObject(Sphere(Vec3(0, 5, -15), 1.00f, PresetMaterials::shinyBlue));
    return std::move(scene);
}

Scene createComplexScene() {
    Scene scene{};
    Light pointLight(Vec3(0, 0, 10), PresetMaterials::pureWhite);
    pointLight.setAmbientIntensity(0.50f);
    pointLight.setDiffuseIntensity(0.75f);
    pointLight.setSpecularIntensity(0.50f);
    scene.addLight(pointLight);
    scene.addSceneObject(Triangle(Vec3(-10, 5, 5), Vec3(10, 5, 5), Vec3(0, 0, -25), PresetMaterials::flatYellow));
    scene.addSceneObject(Sphere(Vec3( 2, -2, -10), 2.00f, PresetMaterials::smoothBlue));
    scene.addSceneObject(Sphere(Vec3(-2,  5, -15), 1.25f, PresetMaterials::roughRed));
    scene.addSceneObject(Sphere(Vec3(-2, 10, -15), 0.50f, PresetMaterials::smoothBlue));
    scene.addSceneObject(Sphere(Vec3( 0,  5, -30), 1.00f, PresetMaterials::shinyBlue));
    scene.addSceneObject(Sphere(Vec3( 0,  5, -15), 1.00f, PresetMaterials::shinyBlue));
    return std::move(scene);
}

int main()
{
    ViewPlane viewPlane{ 10, 10, 10 };
    Timer timer{};
    Tracer tracer{};
    RenderCam cam{ viewPlane };
    FrameBuffer frameBuffer(PresetResolutions::SD_480p, BACKGROUND_COLOR);
    Scene simpleScene = createSimpleScene();
    Scene complexScene = createComplexScene();

    tracer.setShadowColor(SHADOW_COLOR);
    tracer.setBackgroundColor(BACKGROUND_COLOR);
    tracer.setMaxNumReflections(MAX_NUM_REFLECTIONS);

    // trace scene
    std::cout << "Tracing scene...";
    timer.start();
      tracer.trace(cam, simpleScene, frameBuffer);
    timer.stop();
    std::cout << "finished in " << timer.elapsedTime() << "\n";
    
    // output to file
    std::cout << "Writing framebuffer to a "
               << frameBuffer.getImageDescription() << " image file: " << OUTPUT_FILE << ".ppm`" << "...";
    timer.start();
      frameBuffer.writeToFile(OUTPUT_FILE);
    timer.stop();
    std::cout << "finished in " << timer.elapsedTime() << "\n";

    // prompt user to 
    std::cout << "Press ENTER to end...";
    std::cin.get();
}
