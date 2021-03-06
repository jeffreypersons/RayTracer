#include "App.h"
#include <iostream>


Scene createSimpleScene(const Vec3& localOrigin=Vec3::zero()) {
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
    scene.addLight(PointLight(localOrigin + Vec3(0, 100, 25), Palette::white,  0.50f, 1e-10f, 1e-20f));
    scene.addLight(PointLight(localOrigin + Vec3(0,   0, 25), Palette::yellow, 0.50f, 1e-10f, 1e-20f));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, 80,  0), 10.00f, reflectiveRed));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, 55,  0), 15.00f, reflectiveGreen));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, 20,  0), 20.00f, reflectiveBlue));
    return scene;
}

int main() {
    AppOptions options;
    options.imageOutputFile           = "./scene.ppm";
    options.imageOutputSize           = CommonResolutions::HD_4K;
    options.rayTracingReflectionLimit = 3;
    options.skyBoxColor               = Palette::skyBlue;
    options.shadowColor               = Color(0.25f, 0.25f, 0.25f);
    options.cameraNearZ               = 0.50f;
    options.cameraFieldOfView         = 130.0f;
    options.viewTarget                = Vec3(0, 55,  0);
    options.viewOffset                = Vec3(0, 15, 50);

    App app{ createSimpleScene(), options };
    app.run();
}
