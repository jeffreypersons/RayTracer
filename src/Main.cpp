#include "Tracer.h"
#include "Scene.hpp"
#include "StopWatch.hpp"
#include "Presets.hpp"
#include <iostream>


RenderCam createFrontalSceneViewCam(const Vec3& position, float fieldOfView, float viewDist) {
    RenderCam cam{};
    cam.setPosition(position);
    cam.setNearClip(viewDist);
    cam.setFarClip(10000.00f);
    cam.setAspectRatio(1.00f);
    cam.setFieldOfView(fieldOfView);
    cam.setOrientation(Vec3(1, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0));
    return cam;
}
RenderCam createBottomUpSceneViewCam(const Vec3& position, float fieldOfView, float viewDist) {
    RenderCam cam{};
    cam.setPosition(position);
    cam.setNearClip(viewDist);
    cam.setFarClip(10000.00f);
    cam.setAspectRatio(1.00f);
    cam.setFieldOfView(fieldOfView);
    cam.setOrientation(Vec3(0, 0, -1), Vec3(0, -1, 0), Vec3(1, 0, 0));
    return cam;
}

Scene createSimpleScene() {
    Material brightWhite{};
    brightWhite.setColors(0.75 * PresetColors::white, PresetColors::gray, PresetColors::white);

    Material matteBlue{};
    matteBlue.setWeights(1.00f, 0.00f);
    matteBlue.setDiffuseColor(PresetColors::blue);

    Material reflectiveGreen{};
    reflectiveGreen.setWeights(0.50f, 0.50f);
    reflectiveGreen.setDiffuseColor(PresetColors::green);

    Material shinyRed{};
    shinyRed.setWeights(0.95f, 0.05f);
    shinyRed.setSpecularColor(PresetColors::red);

    Scene scene{};
    scene.addLight(Light(Vec3(0, 55, -25), brightWhite));
    scene.addSceneObject(Sphere(Vec3(0, -1500, 0), 1000, matteBlue));
    scene.addSceneObject(Sphere(Vec3(0,    50, 0),   20, reflectiveGreen));
    scene.addSceneObject(Sphere(Vec3(0,    20, 0),    5, shinyRed));
    return scene;
}

int main()
{
    StopWatch stopWatch{};
    stopWatch.start();
    std::cout << "Program started...\n";

    Tracer tracer{};
    tracer.setShadowColor(Color(0.125f, 0.125f, 0.125f));
    tracer.setBackgroundColor(PresetColors::skyBlue);
    tracer.setMaximumallyReflectedColor(PresetColors::pink);
    tracer.setMaxNumReflections(5);
    tracer.setMinTForShadowIntersections(0.01f);

    FrameBuffer frameBuffer(Vec2(1000, 1000), PresetColors::skyBlue);
    Scene simpleScene = createSimpleScene();
    RenderCam frontCam  = createFrontalSceneViewCam( Vec3(0, 50, 50), 90.00f, 0.10f);
    RenderCam bottomCam = createBottomUpSceneViewCam(Vec3(0, 10,  0), 90.00f, 10.00f);
    std::cout << "\nfrontal-" << frontCam  << "\n";
    std::cout << "\nbottom-"  << bottomCam << "\n";

    std::cout << "\nRendering scene from frontal view...";
    tracer.trace(frontCam, simpleScene, frameBuffer);
    frameBuffer.writeToFile("./scene_front-view");

    std::cout << "\nRendering scene from bottom view...";
    tracer.trace(bottomCam, simpleScene, frameBuffer);
    frameBuffer.writeToFile("./scene_bottom-view");

    stopWatch.stop();
    std::cout << "\nProgram finished in " << stopWatch.elapsedTime() << " seconds";
    std::cout << "\nPress ENTER to end...";
    std::cin.get();
}
