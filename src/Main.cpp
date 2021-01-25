#include "Tracer.h"
#include "Scene.hpp"
#include "StopWatch.hpp"
#include "Presets.hpp"
#include <iostream>


// todo: add lookAt() calls and a target ref point in the scene a distZ past the image plane
// todo: add directional constants like WorldUp, etc...
RenderCam createFrontalSceneViewCam(const Vec3& position, float fieldOfView, float viewDist) {
    RenderCam cam{};
    cam.setPosition(position);
    cam.setNearClip(viewDist);
    cam.setFarClip(10000.00f);
    cam.setAspectRatio(1.00f);
    cam.setFieldOfView(fieldOfView);
    cam.setOrientation(Vec3(1, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0));
    return cam;
}
RenderCam createBottomUpSceneViewCam(const Vec3& position, float fieldOfView, float viewDist) {
    RenderCam cam{};
    cam.setPosition(position);
    cam.setNearClip(viewDist);
    cam.setFarClip(10000.00f);
    cam.setAspectRatio(1.00f);
    cam.setFieldOfView(fieldOfView);
    cam.setOrientation(Vec3(0, 0, -1), Vec3(0, -1, 0), Vec3(1, 0, 0));
    return cam;
}

Scene createSimpleScene() {
    Scene scene{};
    Light pointLight(Vec3(0, 55, -25), PresetMaterials::pureWhite);
    pointLight.setAmbientIntensity(0.25f);
    pointLight.setDiffuseIntensity(0.50f);
    pointLight.setSpecularIntensity(0.50f);
    scene.addLight(pointLight);

    // this will be our 'ground'
    scene.addSceneObject(Sphere(Vec3(0, -1500, 0), 1000.00f, Material(
        Color(0.90f, 0.90f, 0.90f),
        Color(0.90f, 0.90f, 0.90f),
        Color(0.90f, 0.90f, 0.90f),
        1.00f, 0.00f
    )));
    scene.addSceneObject(Sphere(Vec3(0, 50, 0), 20, PresetMaterials::flatYellow));
    scene.addSceneObject(Sphere(Vec3(0, 20, 0),  5, PresetMaterials::roughRed));
    return scene;
}

// TODO: LOOK INTO FIXING THE GIANT EARTH THING AND FIGURE OUT WHY IT REFLECT AT TOP, TOO!?!?!?!??!
// todo: look into possible blending issues with colors, and get a better grasp of the best specular/diffuse/ambient
// colors of the material, and what it really SHOULD look like vs actually looks like currently...
// todo: look into blending shadow color instead of overriding it like we currently do...
// todo: look into fixing shadows..you can see a hint of whats wrong by setting minT to tiny value, and the shaded part is more correct
// tracer.setMinTForShadowIntersections(0.00000000000000000000001f);
int main()
{
    StopWatch stopWatch{};
    stopWatch.start();
    std::cout << "Program started...\n";

    Tracer tracer{};
    tracer.setShadowColor(Color(0.125f, 0.125f, 0.125f));
    tracer.setBackgroundColor(PresetColors::skyBlue);
    tracer.setMaximumallyReflectedColor(PresetColors::pink);
    tracer.setMaxNumReflections(5);
    tracer.setMinTForShadowIntersections(0.01f);

    FrameBuffer frameBuffer(Vec2(1000, 1000), PresetColors::skyBlue);
    Scene simpleScene = createSimpleScene();
    RenderCam frontCam  = createFrontalSceneViewCam( Vec3(0, 50, 50), 90.00f, 0.10f);
    RenderCam bottomCam = createBottomUpSceneViewCam(Vec3(0, 10,  0), 90.00f, 10.00f);
    std::cout << "\nfrontal-" << frontCam  << "\n";
    std::cout << "\nbottom-"  << bottomCam << "\n";

    std::cout << "\nRendering scene from frontal view...";
    tracer.trace(frontCam, simpleScene, frameBuffer);
    frameBuffer.writeToFile("./scene_front-view");

    std::cout << "\nRendering scene from bottom view...";
    tracer.trace(bottomCam, simpleScene, frameBuffer);
    frameBuffer.writeToFile("./scene_bottom-view");

    stopWatch.stop();
    std::cout << "\nProgram finished in " << stopWatch.elapsedTime() << " seconds";
    std::cout << "\nPress ENTER to end...";
    std::cin.get();
}
