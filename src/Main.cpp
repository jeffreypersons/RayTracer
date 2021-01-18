#include "Tracer.h"
#include "Scene.hpp"
#include "Timer.hpp"
#include "Presets.hpp"
#include <iostream>


const std::string OUTPUT_FILE = "./scene";
static constexpr Color SHADOW_COLOR     = ColorPresets::darkGrey;
static constexpr Color BACKGROUND_COLOR = ColorPresets::skyBlue;
static constexpr size_t MAX_NUM_REFLECTIONS = 5;

Scene createSimpleScene() {
    Scene scene{};
    Light pointLight(Vec3(0, 1, 0), PresetMaterials::pureWhite);
    pointLight.setAmbientIntensity(0.50f);
    pointLight.setDiffuseIntensity(0.50f);
    pointLight.setSpecularIntensity(0.50f);
    scene.addLight(pointLight);
    scene.addSceneObject(Sphere(Vec3(0, 50, 0), 25, PresetMaterials::flatYellow));
    scene.addSceneObject(Sphere(Vec3(0, 10, 0),  5, PresetMaterials::roughRed));
    return scene;
}

int main()
{
    Timer timer{};
    timer.start();
    std::cout << "Program started...";
    Tracer tracer{};
    RenderCam cam{};
    FrameBuffer frameBuffer(Vec2(1000, 1000), BACKGROUND_COLOR);
    Scene simpleScene = createSimpleScene();
    tracer.setShadowColor(SHADOW_COLOR);
    tracer.setBackgroundColor(BACKGROUND_COLOR);
    tracer.setMaxNumReflections(MAX_NUM_REFLECTIONS);

    // manually set camera to view from below the scene pointed upwards
    cam.setPosition(Vec3(0, -10, 0));
    cam.setViewDistance(10);
    cam.setViewportSize(100, 100);
    cam.setOrientation(Vec3(0, 0, 1), Vec3(0, 1, 0), Vec3(1, 0, 0));

    //cam.lookAt(simpleScene.getObject(0).getCentroid(), Vec3(0, ));
    std::cout << "camera pointed in direction " << cam.getAimDir() << ": " <<
                 "from " << cam.getPosition() << " to " << simpleScene.getObject(0).getCentroid() << "\n";

    tracer.trace(cam, simpleScene, frameBuffer);
    frameBuffer.writeToFile(OUTPUT_FILE);
    timer.stop();

    std::cout << "finished in " << timer.elapsedTime() << "\n";
    std::cout << "Press ENTER to end...";
    std::cin.get();
}
