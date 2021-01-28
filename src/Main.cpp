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
    brightWhite.setColors(Palette::gray, 0.75 * Palette::white, Palette::white);

    Material reflectiveGreen{};
    reflectiveGreen.setWeights(0.50f, 0.50f);
    reflectiveGreen.setDiffuseColor(Palette::green);
    
    Scene scene{};
    scene.addLight(Light(Vec3(0, 55, 0), brightWhite));
    scene.addSceneObject(Sphere(Vec3(0, 35, 15), 20, reflectiveGreen));
    return scene;
}

int main()
{
    std::cout << "Program started...\n";

    Tracer tracer{};
    tracer.setShadowColor(Color(0.125f, 0.125f, 0.125f));
    tracer.setBackgroundColor(Palette::skyBlue);
    tracer.setMaximumallyReflectedColor(Palette::pink);
    tracer.setMaxNumReflections(3);
    tracer.setMinTForShadowIntersections(0.01f);

    FrameBuffer frameBuffer(Vec2(1250, 1250), Palette::skyBlue);
    Scene scene   = createSimpleScene();
    RenderCam cam = createFrontCam( Vec3(0, 40, 50), 90.00f, 5.00f);

    std::cout << "Assembling scene..\n.." << scene << "\n\n";
    std::cout << "Configuring frontal-view cams\n  " << cam << "\n\n";

    tracer.trace(cam, scene, frameBuffer);
    frameBuffer.writeToFile("./scene");
    std::cout << "  wrote rendered scene as image output to file '" << "./scene_front-view" << "'\n";

    std::cout << "Press ENTER to end...";
    std::cin.get();
}
