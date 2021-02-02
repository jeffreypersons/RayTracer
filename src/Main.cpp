#include "Tracer.h"
#include "Scene.hpp"
#include "StopWatch.hpp"
#include <iostream>


RenderCam createCam(const Vec3& position, float fieldOfView, float viewDist, const Vec3& target) {
    RenderCam cam{};
    cam.setPosition(position);
    cam.setNearClip(viewDist);
    cam.setFarClip(10000.00f);
    cam.setAspectRatio(1.00f);
    cam.setFieldOfView(fieldOfView);
    cam.lookAt(target);
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

    Scene scene = createSimpleScene();
    FrameBuffer frameBuffer(Vec2(1250, 1250), Palette::skyBlue);

    Vec3 target = scene.getObject(0).getCentroid();
    RenderCam frontCam  = createCam(Vec3(0,  50,  50), 100.00f, 5.00f, target);
    RenderCam behindCam = createCam(Vec3(0,  50, -50), 100.00f, 5.00f, target);
    RenderCam topCam    = createCam(Vec3(0, 100,   0), 100.00f, 5.00f, target);
    RenderCam bottomCam = createCam(Vec3(0,   0,   0), 100.00f, 5.00f, target);

    std::cout << "Initializing target-" << frameBuffer << "\n\n";
    std::cout << "Assembling "          << scene       << "\n\n";
    std::cout << "Configuring front-"   << frontCam    << "\n\n";
    std::cout << "Configuring behind-"  << behindCam   << "\n\n";
    std::cout << "Configuring top-"     << topCam      << "\n\n";
    std::cout << "Configuring bottom-"  << bottomCam   << "\n\n";
    tracer.trace(frontCam,  scene, frameBuffer); frameBuffer.writeToFile("./scene-front");
    tracer.trace(behindCam, scene, frameBuffer); frameBuffer.writeToFile("./scene-back");
    tracer.trace(topCam,    scene, frameBuffer); frameBuffer.writeToFile("./scene-top");
    tracer.trace(bottomCam, scene, frameBuffer); frameBuffer.writeToFile("./scene-bottom");

    std::cout << "Press ENTER to end...";
    std::cin.get();
}
