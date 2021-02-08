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
    cam.overrideViewportSize(Vec2(100, 25), 20.00f, 1000.00f);
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
    FrameBuffer frameBuffer{ CommonResolutions::HD_1080p, Palette::skyBlue };
    RenderCam frontCam  = createCam(Vec3(0,  50,  50), 110.00f, 5.00f, scene.getObject(0).getCentroid(), frameBuffer.getAspectRatio());
    
    std::cout << "Initializing target-" << frameBuffer << "\n\n";
    std::cout << "Assembling "          << scene       << "\n\n";
    std::cout << "Configuring cam-"     << frontCam    << "\n\n";
    tracer.trace(frontCam,  scene, frameBuffer); frameBuffer.writeToFile("./scene");
    
    std::cout << "Press ENTER to end...";
    std::cin.get();
}
