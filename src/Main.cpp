#include "Tracer.h"
#include "Scene.hpp"
#include "StopWatch.hpp"
#include "Presets.hpp"
#include <iostream>


RenderCam createFrontalSceneViewCam(float viewWidth, float viewHeight, float viewDist) {
    RenderCam cam{};
    cam.setPosition(Vec3(0.00f, 0.00f, viewDist));
    cam.setNearClip(Math::abs(viewDist));
    cam.setFieldOfView(60);
    cam.setOrientation(Vec3(0, 0, -1), Vec3(1, 0, 0), Vec3(0, 1, 0));
    return cam;
}
RenderCam createBottomUpSceneViewCam(float viewWidth, float viewHeight, float viewDist) {
    RenderCam cam{};
    cam.setPosition(Vec3(0.00f, 0.00f, viewDist));
    cam.overrideViewportSize(100, 100, 100);
    cam.setOrientation(Vec3(0, 0, -1), Vec3(1, 0, 0), Vec3(0, 1, 0));

    /*
    RenderCam cam{};
    cam.setPosition(Vec3(0.00f, -viewDist, 0.00f));
    cam.setNearClip(Math::abs(viewDist));
    cam.setFieldOfView(60);
    cam.setOrientation(Vec3(0, 1, 0), Vec3(1, 0, 0), Vec3(0, 0, 1));
    */
    return cam;
}

Scene createSimpleScene() {
    Scene scene{};
    Light pointLight(Vec3(0, 100, 0), PresetMaterials::pureWhite);
    pointLight.setAmbientIntensity(0.50f);
    pointLight.setDiffuseIntensity(0.50f);
    pointLight.setSpecularIntensity(0.50f);
    scene.addLight(pointLight);
    scene.addSceneObject(Sphere(Vec3(0, 50, 0), 50, PresetMaterials::flatYellow));
    scene.addSceneObject(Sphere(Vec3(0, 10, 0),  5, PresetMaterials::roughRed));
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
    RenderCam frontCam  = createFrontalSceneViewCam( 100, 100, 100);
    RenderCam bottomCam = createBottomUpSceneViewCam(100, 100, 100);

    std::cout << "rendering scene from frontal view...\n";
    tracer.trace(frontCam, simpleScene, frameBuffer);
    frameBuffer.writeToFile("./scene_front-view");

    std::cout << "rendering scene from bottom view...\n";
    tracer.trace(bottomCam, simpleScene, frameBuffer);
    frameBuffer.writeToFile("./scene_bottom-view");

    stopWatch.stop();
    std::cout << "finished in " << stopWatch.elapsedTime() << "\n";
    std::cout << "Press ENTER to end...";
    std::cin.get();
}
