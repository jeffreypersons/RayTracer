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

// TODO: look into fresnel, better/smoother fall-off-effect, etc...
// TODO: look into why the specular highlights seem off center...
// ......see https://stackoverflow.com/questions/33054399/raytracing-lighting-equations
Scene createSimpleScene() {
    Material brightWhite{};
    brightWhite.setColors(Palette::gray, Palette::white, Palette::white);

    Material matteGreen{};
    matteGreen.setWeights(1.00f, 0.00f);
    matteGreen.setAmbientColor(Palette::lightGreen);
    matteGreen.setDiffuseColor(Palette::green);
    matteGreen.setSpecularColor(Palette::green);

    Scene scene{};
    scene.addLight(Light(Vec3(0, 100, 25), brightWhite));
    scene.addSceneObject(Sphere(Vec3(0, 50, 0), 25, matteGreen));
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
    RenderCam cam = createFrontCam(Vec3(0, 50, 50), 100.00f, 5.00f);

    tracer.trace(cam, scene, frameBuffer);
    frameBuffer.writeToFile("./scene");

    std::cout << cam << "\n\n" << scene.getLight(0) << "\n\n" << scene.getObject(0) << "\n\n" << "Program finished...\n";
    std::cin.get();
}
