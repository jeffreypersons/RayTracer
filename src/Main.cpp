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
    Material matteGreen{};
    matteGreen.setWeights(0.82, 0.18);
    matteGreen.setColors(Palette::darkGreen, Palette::green, Palette::lightGreen);
    matteGreen.setShininess(5);

    Material reflectiveGreen{};
    reflectiveGreen.setWeights(0.10, 0.90);
    reflectiveGreen.setColors(Palette::darkGreen, Palette::green, Palette::lightGreen);
    reflectiveGreen.setShininess(5);

    Scene scene{};
    scene.addLight(PointLight(localOrigin + Vec3(0,   100, 25), Palette::white, 1.50f, 1e-10f, 1e-20f));
    scene.addLight(PointLight(localOrigin + Vec3(0,     0, 25), Palette::white, 1.50f, 1e-10f, 1e-20f));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0,  20,  0), 10.0, matteGreen));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0,   0,  0),  5.0, reflectiveGreen));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, -20,  0), 12.5, reflectiveGreen));
    return scene;
}


int main() {
    std::cout << "Program started...\n\n";
    Tracer tracer{};
    tracer.setBackgroundColor(Palette::skyBlue);
    tracer.setShadowColor(Color(0.125f, 0.125f, 0.125f));
    tracer.setMaxNumReflections(3);
    tracer.setTracingBias(1e-05f);

    Vec3 localOrigin{ 0, 50, 0 };
    Scene scene = createSimpleScene(localOrigin);
    FrameBuffer frameBuffer{ CommonResolutions::HD_1080p, Palette::skyBlue };
    RenderCam frontCam    = createCam(localOrigin + Vec3(0,   0,  50), 110.00f, 1.00f, localOrigin, frameBuffer.getAspectRatio());
    RenderCam frontTopCam = createCam(localOrigin + Vec3(0,  50,  25), 110.00f, 1.00f, localOrigin, frameBuffer.getAspectRatio());
    RenderCam behindCam   = createCam(localOrigin + Vec3(0,   0, -50), 110.00f, 1.00f, localOrigin, frameBuffer.getAspectRatio());
    RenderCam topCam      = createCam(localOrigin + Vec3(0,  50,   0), 110.00f, 1.00f, localOrigin, frameBuffer.getAspectRatio());
    RenderCam bottomCam   = createCam(localOrigin + Vec3(0, -50,   0), 110.00f, 1.00f, localOrigin, frameBuffer.getAspectRatio());
    
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
