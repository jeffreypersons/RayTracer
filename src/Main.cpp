#include "Tracer.h"
#include "Scene.hpp"
#include "StopWatch.hpp"
#include <iostream>


RenderCam createFrontalSceneViewCam(const Vec3& position, float fieldOfView, float viewDist) {
    RenderCam cam{};
    cam.setPosition(position);
    cam.setNearClip(viewDist);
    cam.setFarClip(10000.00f);
    cam.setAspectRatio(1.00f);
    cam.setFieldOfView(fieldOfView);
    cam.setOrientation(Vec3::right(), Vec3::up(), Vec3::ahead());
    return cam;
}
RenderCam createBottomUpSceneViewCam(const Vec3& position, float fieldOfView, float viewDist) {
    RenderCam cam{};
    cam.setPosition(position);
    cam.setNearClip(viewDist);
    cam.setFarClip(10000.00f);
    cam.setAspectRatio(1.00f);
    cam.setFieldOfView(fieldOfView);
    cam.setOrientation(Vec3::ahead(), Vec3::right(), Vec3::up());
    return cam;
}

Scene createSimpleScene() {
    Material brightWhite{};
    brightWhite.setColors(Palette::gray, 0.75 * Palette::white, Palette::white);

    Material matteBlue{};
    matteBlue.setWeights(1.00f, 0.00f);
    matteBlue.setAmbientColor(Palette::blue);
    matteBlue.setDiffuseColor(Palette::blue);
    matteBlue.setSpecularColor(Palette::white);

    Material reflectiveGreen{};
    reflectiveGreen.setWeights(0.50f, 0.50f);
    reflectiveGreen.setDiffuseColor(Palette::green);

    Material shinyRed{};
    shinyRed.setWeights(0.95f, 0.05f);
    shinyRed.setSpecularColor(Palette::red);

    Scene scene{};
    scene.addLight(Light(Vec3(0, 55, -25), brightWhite));
    scene.addSceneObject(Sphere(Vec3(0, -1500, 0), 1000, matteBlue));
    scene.addSceneObject(Sphere(Vec3(0,    50, 0),   20, reflectiveGreen));
    scene.addSceneObject(Sphere(Vec3(0,    20, 0),    5, shinyRed));
    
    std::cout << "\n\nmatte-blue-"     << matteBlue       << "\n";
    std::cout << "\nreflective-green-" << reflectiveGreen << "\n";
    std::cout << "\nshiny-red-"        << shinyRed        << "\n\n";

    return scene;
}

int main()
{
    StopWatch stopWatch{};
    stopWatch.start();
    std::cout << "Program started...\n";

    Tracer tracer{};
    tracer.setShadowColor(Color(0.125f, 0.125f, 0.125f));
    tracer.setBackgroundColor(Palette::skyBlue);
    tracer.setMaximumallyReflectedColor(Palette::pink);
    tracer.setMaxNumReflections(3);
    tracer.setMinTForShadowIntersections(0.01f);

    FrameBuffer frameBuffer(Vec2(1000, 1000), Palette::skyBlue);
    Scene simpleScene   = createSimpleScene();
    RenderCam frontCam  = createFrontalSceneViewCam( Vec3(0, 50, 50), 90.00f,  1.00f);
    RenderCam bottomCam = createBottomUpSceneViewCam(Vec3(0, 10,  0), 90.00f, 10.00f);

    std::cout << "Initializing target buffer..\n  " << frameBuffer << "\n\n";
    std::cout << "Assembling scene..\n.."           << simpleScene << "\n\n";
    std::cout << "Configuring frontal and bottom-up view cams\n  " << frontCam << "\n  " << bottomCam << "\n\n";

    std::cout << "Rendering scene from multiple perspectives..\n";
    tracer.trace(frontCam, simpleScene, frameBuffer);
    frameBuffer.writeToFile("./scene_front-view");
    std::cout << "  wrote rendered scene as image output to file '" << "./scene_front-view" << "'\n";

    tracer.trace(bottomCam, simpleScene, frameBuffer);
    frameBuffer.writeToFile("./scene_bottom-view");
    std::cout << "  wrote rendered scene as image output to file '" << "./scene_bottom-view" << "'\n\n";
    
    stopWatch.stop();
    std::cout << "Program finished in " << stopWatch.elapsedTime() << " seconds\n";
    std::cout << "Press ENTER to end...";
    std::cin.get();
}
