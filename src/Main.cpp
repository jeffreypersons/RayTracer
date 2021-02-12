#include "Tracer.h"
#include "Scene.hpp"
#include "StopWatch.hpp"
#include <iostream>


RenderCam createCam(const Vec3& position, float fieldOfView, float viewDist, const Vec3& target, float aspectRatio) {
    RenderCam cam{};
    cam.setPosition(position);
    cam.setNearClip(viewDist);
    cam.setAspectRatio(aspectRatio);
    cam.setFieldOfView(fieldOfView);
    cam.lookAt(target);
    return cam;
}

Scene createSimpleScene() {
    Material matteGreen{};
    matteGreen.setWeights(0.90f, 0.10f);
    matteGreen.setAmbientColor(Palette::lightGreen);
    matteGreen.setDiffuseColor(Palette::green);
    matteGreen.setSpecularColor(Palette::yellow);
    
    Scene scene{};
    scene.addLight(PointLight(Vec3(0, 100, 25), Palette::white, -1, 0, 0));
    scene.addSceneObject(Sphere(Vec3(0, 50, 0), 25, matteGreen));
    return scene;
}


int main() {
    std::cout << "Program started...\n\n";
    Tracer tracer{};
    tracer.setBackgroundColor(Palette::skyBlue);
    tracer.setShadowColor(Color(0.125f, 0.125f, 0.125f));
    tracer.setMaximumallyReflectedColor(Palette::black);

    Scene scene = createSimpleScene();
    FrameBuffer frameBuffer{ CommonResolutions::HD_1080p, Palette::skyBlue };
    Vec3 target = scene.getObject(0).getCentroid();
    RenderCam frontCam  = createCam(Vec3(0,  50,  50), 110.00f, 5.00f, target, frameBuffer.getAspectRatio());
    RenderCam behindCam = createCam(Vec3(0,  50, -50), 110.00f, 5.00f, target, frameBuffer.getAspectRatio());
    RenderCam topCam    = createCam(Vec3(0, 100,   0), 110.00f, 5.00f, target, frameBuffer.getAspectRatio());
    RenderCam bottomCam = createCam(Vec3(0, -50,   0), 110.00f, 5.00f, target, frameBuffer.getAspectRatio());

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
