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

Scene createSimpleScene(const Vec3& localOrigin) {
    Material reflectiveRed{};
    reflectiveRed.setWeights(0.10f, 0.90f);
    reflectiveRed.setColors(Palette::darkRed, Palette::red, Palette::orangeRed);
    reflectiveRed.setShininess(15);

    Material reflectiveGreen{};
    reflectiveGreen.setWeights(0.10f, 0.90f);
    reflectiveGreen.setColors(Palette::darkGreen, Palette::green, Palette::lightGreen);
    reflectiveGreen.setShininess(10);

    Material reflectiveBlue{};
    reflectiveBlue.setWeights(0.10f, 0.90f);
    reflectiveBlue.setColors(Palette::darkBlue, Palette::blue, Palette::lightBlue);
    reflectiveBlue.setShininess(5);

    Scene scene{};
    scene.addLight(PointLight(localOrigin + Vec3(0, 100, 25), Palette::white, 0.50f, 1e-10f, 1e-20f));
    scene.addLight(PointLight(localOrigin + Vec3(0,   0, 25), Palette::yellow, 0.50f, 1e-10f, 1e-20f));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, 80,  0), 10.00f, reflectiveRed));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, 55,  0), 15.00f, reflectiveGreen));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, 20,  0), 20.00f, reflectiveBlue));
    return scene;
}


int main() {
    std::cout << "Program started...\n\n";
    Tracer tracer{};
    tracer.setBackgroundColor(Palette::skyBlue);
    tracer.setShadowColor(Color(0.125, 0.125, 0.125));
    tracer.setMaxNumReflections(3);
    tracer.setShadowBias(0.02f);
    tracer.setReflectionBias(0.08f);

    Vec3 eyeTarget{ 0, 50, 0 };
    Vec3 sceneOrigin{ 0, 0, 0 };
    Scene scene = createSimpleScene(sceneOrigin);
    FrameBuffer frameBuffer{ CommonResolutions::HD_4K, Palette::skyBlue };
    RenderCam frontCam    = createCam(eyeTarget + Vec3(0,   0,  50), 120.00f, 0.50f, eyeTarget, frameBuffer.getAspectRatio());
    RenderCam frontTopCam = createCam(eyeTarget + Vec3(0,  50,  25), 120.00f, 0.50f, eyeTarget, frameBuffer.getAspectRatio());
    RenderCam behindCam   = createCam(eyeTarget + Vec3(0,   0, -50), 120.00f, 0.50f, eyeTarget, frameBuffer.getAspectRatio());
    RenderCam topCam      = createCam(eyeTarget + Vec3(0,  50,   0), 120.00f, 0.50f, eyeTarget, frameBuffer.getAspectRatio());
    RenderCam bottomCam   = createCam(eyeTarget + Vec3(0, -50,   0), 120.00f, 0.50f, eyeTarget, frameBuffer.getAspectRatio());
    
    std::cout << "Initializing target-"   << frameBuffer << "\n\n";
    std::cout << "Initializing ray-"      << tracer      << "\n\n";
    std::cout << "Assembling "            << scene       << "\n\n";
    std::cout << "Configuring front-"     << frontCam    << "\n\n";
    std::cout << "Configuring front-top-" << frontTopCam << "\n\n";
    std::cout << "Configuring behind-"    << behindCam   << "\n\n";
    std::cout << "Configuring top-"       << topCam      << "\n\n";
    std::cout << "Configuring bottom-"    << bottomCam   << "\n\n";
    tracer.trace(frontCam,    scene, frameBuffer); frameBuffer.writeToFile("./scene-front");
    tracer.trace(frontTopCam, scene, frameBuffer); frameBuffer.writeToFile("./scene-front-top");
    tracer.trace(behindCam,   scene, frameBuffer); frameBuffer.writeToFile("./scene-back");
    tracer.trace(topCam,      scene, frameBuffer); frameBuffer.writeToFile("./scene-top");
    tracer.trace(bottomCam,   scene, frameBuffer); frameBuffer.writeToFile("./scene-bottom");
    std::cout << "Press ENTER to end...";
    std::cin.get();
}
