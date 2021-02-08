#include "Tracer.h"
#include "Scene.hpp"
#include "StopWatch.hpp"
#include <iostream>


/*
Configuring bottom-RenderCam(
    position:(0.00,-50.00,0.00),
    Orientation{
        right-axis:(-1.00,0.00,0.00),
        upward-axis:(0.00,0.00,-1.00),
        forward-axis:(0.00,1.00,0.00)},
    ImagePlane{
        aspect-ratio:4.00,
        field-of-view-x:140.03,
        field-of-view-y:69.02,
        viewport-width:110.00,
        viewport-height:27.50
     },
     Clipping{near-plane-z:20.00,far-plane-z:200.00})
*/
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
    Tracer tracer{}; tracer.setBackgroundColor(Palette::skyBlue);

    Scene scene = createSimpleScene();
    FrameBuffer frameBuffer{ Vec2(1000, 1000), Palette::skyBlue };
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
