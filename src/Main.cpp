#include "Tracer.h"
#include "Scene.hpp"
#include "Timer.hpp"
#include "Presets.hpp"
#include <iostream>


const std::string OUTPUT_FILE = "./scene";
static constexpr Color SHADOW_COLOR{ 0.05f, 0.05f, 0.05f };
static constexpr Color BACKGROUND_COLOR{ .529f, 0.808f, 0.922f };
static constexpr size_t MAX_NUM_REFLECTIONS = 5;

Scene createSimpleScene() {
    Scene scene{};
    Light pointLight(Vec3(0, 5, 0), PresetMaterials::pureWhite);
    pointLight.setAmbientIntensity(0.50f);
    pointLight.setDiffuseIntensity(0.50f);
    pointLight.setSpecularIntensity(0.50f);
    scene.addLight(pointLight);
    scene.addSceneObject(Sphere(Vec3(0, -5, 0), 2.00f, PresetMaterials::flatYellow));
    return scene;
}

int main()
{
    Timer timer{};
    timer.start();
    std::cout << "Program started...";
    Tracer tracer{};
    RenderCam cam{ Viewport(10, 10, 10) };
    FrameBuffer frameBuffer(Vec2(750, 750), BACKGROUND_COLOR);
    Scene simpleScene = createSimpleScene();
    tracer.setShadowColor(SHADOW_COLOR);
    tracer.setBackgroundColor(BACKGROUND_COLOR);
    tracer.setMaxNumReflections(MAX_NUM_REFLECTIONS);

    cam.setPosition(Vec3(0, 0, -10));
    tracer.trace(cam, simpleScene, frameBuffer);
    frameBuffer.writeToFile(OUTPUT_FILE);
    timer.stop();

    std::cout << "finished in " << timer.elapsedTime() << "\n";
    std::cout << "Press ENTER to end...";
    std::cin.get();
}
