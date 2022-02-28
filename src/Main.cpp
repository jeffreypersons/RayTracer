#include "App.hpp"
#include "Math.hpp"
#include <iostream>
#include <random>

// TODO: maybe put this somewhere else
bool isSphereSphereCollision(const Sphere& a, const Sphere& b) {
    return (Math::magnitude(a.center() - b.center()) < (a.radius() + b.radius()));
}

bool sphereCollidesWithAnyOtherSpheres(const std::vector<Sphere>& spheres, const Sphere& sphereInQuestion) {
    for (int i = 0; i < spheres.size(); i++) {
        if (isSphereSphereCollision(sphereInQuestion, spheres.at(i)))
            return true;
    }
    return false;
}

void addGround(Scene& scene) {
    Color groundColor = Palette::gray;
    float groundIntrinsivity = 0.7;
    float groundReflectivity = 1.0f - groundIntrinsivity;
    Material groundMaterial{};
    groundMaterial.setWeights(groundIntrinsivity, groundReflectivity);
    groundMaterial.setColors(groundColor, groundColor, groundColor);
    groundMaterial.setShininess(1);

    float L = 1000.0f;
    Vec3 v11{-L, 0.0f, -L};
    Vec3 v12{ L, 0.0f, -L};
    Vec3 v13{ L, 0.0f,  L};
    scene.addSceneObject(Triangle(v11, v12, v13, groundMaterial));

    Vec3 v21{-L, 0.0f, -L};
    Vec3 v22{ L, 0.0f,  L};
    Vec3 v23{-L, 0.0f,  L};
    scene.addSceneObject(Triangle(v21, v22, v23, groundMaterial));
}

Scene createSimpleScene(const Vec3& localOrigin=Vec3::zero()) {
    Material reflectiveRed{};
    reflectiveRed.setWeights(0.50f, 0.50f);
    reflectiveRed.setColors(Palette::darkRed, Palette::red, Palette::orangeRed);
    reflectiveRed.setShininess(10);

    Material reflectiveGreen{};
    reflectiveGreen.setWeights(0.50f, 0.50f);
    reflectiveGreen.setColors(Palette::darkGreen, Palette::green, Palette::lightGreen);
    reflectiveGreen.setShininess(10);

    Material reflectiveBlue{};
    reflectiveBlue.setWeights(0.50f, 0.50f);
    reflectiveBlue.setColors(Palette::darkBlue, Palette::blue, Palette::lightBlue);
    reflectiveBlue.setShininess(10);

    Scene scene{};
    scene.addLight(PointLight(localOrigin + Vec3(0, 55, 50), Palette::antiqueWhite));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, 80, 0), 10.00f, reflectiveRed));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, 55, 0), 15.00f, reflectiveGreen));
    scene.addSceneObject(Sphere(localOrigin + Vec3(0, 20, 0), 20.00f, reflectiveBlue));
    return scene;
}

Scene createRandomFloatingScene() {
    Scene scene{};
    scene.addLight(PointLight(Vec3(0.0f, 0.0f, 0.0f), Palette::antiqueWhite));

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen{rd()}; // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> x_dis(-100.0, 100.0);
    std::uniform_real_distribution<float> y_dis(0.0, 100.0);
    std::uniform_real_distribution<float> z_dis(10.0, 100.0);
    std::uniform_real_distribution<float> r_dis(1.0, 10.0);
    std::uniform_real_distribution<float> c_dis(0.0, 1.0);

    const size_t N_SPHERES = 100;
    std::vector<Sphere> spheres;
    spheres.reserve(N_SPHERES);

    for (int i = 0; i < N_SPHERES; i++) {
        bool validSphere = false;
        while (!validSphere) {
            float x = x_dis(gen);
            float y = y_dis(gen);
            float z = z_dis(gen);
            float R = r_dis(gen);

            float r = c_dis(gen);
            float g = c_dis(gen);
            float b = c_dis(gen);

            float intrinsity = c_dis(gen);
            float reflectivity = 1.0f - intrinsity;

            Color c{r,g,b};
            Material material{};
            material.setWeights(intrinsity, reflectivity);
            material.setColors(c, c, c);
            material.setShininess(10);

            Sphere s{Vec3(x, y, z), R, material};

            if (!sphereCollidesWithAnyOtherSpheres(spheres, s)) {
                validSphere = true;
                spheres.push_back(s);
            }
        }
    }

    for (int i = 0; i < N_SPHERES; i++) {
        scene.addSceneObject(std::move(spheres.at(i)));
    }

    return scene;
}

Scene createRandomGroundScene() {
    Scene scene{};
    scene.addLight(PointLight(Vec3(0.0f, 0.0f, 0.0f), Palette::antiqueWhite));

    addGround(scene);

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen{rd()}; // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> x_dis(-100.0, 100.0);
    std::uniform_real_distribution<float> z_dis(10.0, 100.0);
    std::uniform_real_distribution<float> r_dis(1.0, 10.0);
    std::uniform_real_distribution<float> c_dis(0.0, 1.0);

    const size_t N_SPHERES = 100;
    std::vector<Sphere> spheres;
    spheres.reserve(N_SPHERES);

    for (int i = 0; i < N_SPHERES; i++) {
        bool validSphere = false;
        while (!validSphere) {
            float R = r_dis(gen);
            float x = x_dis(gen);
            float y = R;
            float z = z_dis(gen);

            float r = c_dis(gen);
            float g = c_dis(gen);
            float b = c_dis(gen);

            float intrinsity = c_dis(gen);
            float reflectivity = 1.0f - intrinsity;

            Color c{r,g,b};
            Material material{};
            material.setWeights(intrinsity, reflectivity);
            material.setColors(c, c, c);
            material.setShininess(10);

            Sphere s{Vec3(x, y, z), R, material};

            if (!sphereCollidesWithAnyOtherSpheres(spheres, s)) {
                validSphere = true;
                spheres.push_back(std::move(s));
            }
        }
    }

    for (int i = 0; i < N_SPHERES; i++) {
        scene.addSceneObject(std::move(spheres.at(i)));
    }

    return scene;
}

Scene createTriangleScene() {
    Scene scene{};

    Color Color = Palette::gray;
    float intrinsity = 0.8;
    float reflectivity = 1.0f - intrinsity;
    Material Material{};
    Material.setWeights(intrinsity, reflectivity);
    Material.setColors(Color, Color, Color);
    Material.setShininess(1);

    float L = 10.0;

    Vec3 v1{0.0f, 0.0f, 0.0f};
    Vec3 v2{L,    0.0f, 0.0f};
    Vec3 v3{L,    L,    0.0f};
    scene.addSceneObject(Triangle(v1, v2, v3, Material));

    return scene;
}

Scene createSimpleGroundScene() {
    Scene scene{};
    scene.addLight(PointLight(Vec3(0.0f, 0.0f, 0.0f), Palette::antiqueWhite));

    addGround(scene);

    Material reflectiveBlue{};
    reflectiveBlue.setWeights(0.50f, 0.50f);
    reflectiveBlue.setColors(Palette::darkBlue, Palette::blue, Palette::lightBlue);
    reflectiveBlue.setShininess(10);

    float R = 30.0f;
    scene.addSceneObject(Sphere(Vec3(0.0f, R, 0.0f), R, reflectiveBlue));

    return scene;
}

int main() {
    AppOptions options;
    options.imageOutputFile           = "./scene.ppm";
    options.imageOutputSize           = CommonResolutions::HD_4K;
    options.rayTracingReflectionLimit = 10;
    options.skyBoxColor               = Palette::skyBlue;
    options.shadowColor               = Color(0.25f, 0.25f, 0.25f);
    options.viewTarget                = Vec3(0, 0, 0);
    options.viewOffset                = Vec3(0, 30, 150);

    App app{ createRandomGroundScene(), options };
    app.run();
}
