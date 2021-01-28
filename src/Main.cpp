#include "Tracer.h"
#include "Scene.hpp"
#include "StopWatch.hpp"
#include <iostream>


RenderCam createFrontCam(const Vec3& position, float fieldOfView, float viewDist) {
    RenderCam cam{};
    cam.setPosition(position);
    cam.setNearClip(viewDist);
    cam.setFarClip(10000.00f);
    cam.setAspectRatio(1.00f);
    cam.setFieldOfView(fieldOfView);
    cam.setOrientation(Vec3::right(), Vec3::up(), Vec3::ahead());
    return cam;
}

Scene createSimpleScene() {
    Material brightWhite{};
    std::cout << "before set" << brightWhite << "\n";
    brightWhite.setColors(Palette::white, 0.75 * Palette::white, Palette::white);
    std::cout << "after set" << brightWhite << "\n";

    Material reflectiveGreen{};
    reflectiveGreen.setWeights(0.50f, 0.50f);
    reflectiveGreen.setDiffuseColor(Palette::green);
    
    Scene scene{};
    scene.addLight(Light(Vec3(0, 100, 0), brightWhite));
    scene.addSceneObject(Sphere(Vec3(0, 35, 15), 20, reflectiveGreen));
    return scene;
}

int main()
{
    std::cout << "Program started...\n\n";

    Tracer tracer{};
    tracer.setBackgroundColor(Palette::skyBlue);
    tracer.setMinTForShadowIntersections(0.01f);

    FrameBuffer frameBuffer(Vec2(1250, 1250), Palette::skyBlue);
    Scene scene   = createSimpleScene();
    RenderCam cam = createFrontCam(Vec3(0, 40, 50), 100.00f, 5.00f);

    tracer.trace(cam, scene, frameBuffer);
    frameBuffer.writeToFile("./scene");

    std::cout << cam << "\n\n" << scene.getLight(0) << "\n\n" << scene.getObject(0) << "\n\n" << "Program finished...\n";
    std::cin.get();
}
