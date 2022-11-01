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

void addGround(Scene& scene, const Material& groundMaterial) {
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

Scene createRandomGroundScene() {
    Scene scene{};
    scene.addAmbientLight({0.0f, 0.0f, 0.0f});
    scene.addLight(PointLight(Vec3(50.0f, 200.0f, -10.0f), Palette::antiqueWhite));
    scene.addLight(PointLight(Vec3(-50.0f, 200.0f, -10.0f), Palette::antiqueWhite));

    Color groundColor = Palette::darkGreen;
    float groundReflectivity = 0.15f;
    Material groundMaterial{};
    groundMaterial.setReflectivity(groundReflectivity);
    groundMaterial.setColor(groundColor);

    addGround(scene, groundMaterial);

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

            float reflectivity = c_dis(gen);

            Color c{r,g,b};
            Material material{};
            material.setReflectivity(reflectivity);
            material.setColor(c);

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

Scene createRandomGroundSceneRefraction() {
    Scene scene{};
    scene.addAmbientLight({0.0f, 0.0f, 0.0f});
    scene.addLight(PointLight(Vec3(50.0f, 200.0f, -10.0f), Palette::antiqueWhite));
    scene.addLight(PointLight(Vec3(-50.0f, 200.0f, -10.0f), Palette::antiqueWhite));

    Color groundColor = Palette::darkGreen;
    float groundReflectivity = 0.15f;
    Material groundMaterial{};
    groundMaterial.setReflectivity(groundReflectivity);
    groundMaterial.setColor(groundColor);

    addGround(scene, groundMaterial);

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen{rd()}; // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> x_dis(-100.0, 100.0);
    std::uniform_real_distribution<float> z_dis(10.0, 100.0);
    std::uniform_real_distribution<float> r_dis(1.0, 10.0);
    std::uniform_real_distribution<float> c_dis(0.0, 1.0);
    std::uniform_real_distribution<float> ior_dis(1.0, 3.0);

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

            float reflectivity = c_dis(gen);
            float ior = ior_dis(gen);

            Color c{r,g,b};
            Material material{};
            material.setReflectivity(reflectivity);
            material.setColor(c);
            material.setIndexOfRefraction(ior);

            Sphere s{Vec3(x, y, z), R, material};

            if (!sphereCollidesWithAnyOtherSpheres(spheres, s)) {
                validSphere = true;
            }
        }
    }

    for (int i = 0; i < N_SPHERES; i++) {
        scene.addSceneObject(std::move(spheres.at(i)));
    }

    return scene;
}

Scene createSimpleGroundScene() {
    Scene scene{};
    scene.addAmbientLight({0.0f, 0.0f, 0.0f});
    scene.addLight(PointLight(Vec3(50.0f, 200.0f, -10.0f), Palette::antiqueWhite));
    scene.addLight(PointLight(Vec3(-50.0f, 200.0f, -10.0f), Palette::antiqueWhite));

    Material groundMaterial{MaterialSample::matteGray};
    addGround(scene, groundMaterial);

    float R;
    R = 30.0f;
    scene.addSceneObject(Sphere(Vec3(-1.5f*R, R, 0.0f), R, MaterialSample::reflectiveBlue));
    R = 20.0f;
    scene.addSceneObject(Sphere(Vec3(1.5f*R, R, 0.0f), R, MaterialSample::reflectiveRed));

    return scene;
}

Scene createSimpleGroundSceneRefraction() {
    Scene scene{};
    scene.addAmbientLight({0.0f, 0.0f, 0.0f});
    scene.addLight(PointLight(Vec3(50.0f, 200.0f, -10.0f), Palette::antiqueWhite));
    scene.addLight(PointLight(Vec3(-50.0f, 200.0f, -10.0f), Palette::antiqueWhite));

    Material groundMaterial{MaterialSample::matteGray};
    addGround(scene, groundMaterial);

    Material refractive{};
    refractive.setColor(Palette::white);
    refractive.setReflectivity(0.3f);
    refractive.setShininess(5);
    refractive.setTransparency(0.8f);
    refractive.setIndexOfRefraction(1.2f);

    float R;

    R = 30.0f;
    scene.addSceneObject(Sphere(Vec3(-1.5f*R, R, 0.0f), R, MaterialSample::reflectiveBlue));
    R = 15.0f;
    scene.addSceneObject(Sphere(Vec3(1.5f*R, R, 0.0f), R, MaterialSample::reflectiveRed));

    R = 15.0f;
    refractive.setIndexOfRefraction(1.2f);
    scene.addSceneObject(Sphere(Vec3(0.5f*R, R, R), R, refractive));
    R = 15.0f;
    refractive.setIndexOfRefraction(1.4f);
    scene.addSceneObject(Sphere(Vec3(-0.5f*R, R, -R), R, refractive));

    return scene;
}

int main() {
    AppOptions options;
    options.imageOutputFile           = "./scene.ppm";
    options.imageOutputSize           = CommonResolutions::HD_4K;
    options.rayTracingReflectionLimit = 5;
    options.skyBoxColor               = Palette::skyBlue;
    options.shadowColor               = Color(0.125f, 0.125f, 0.125f);
    options.viewTarget                = Vec3(0, 0, 0);
    options.viewOffset                = Vec3(0, 50, 150);

    App app{ createSimpleGroundSceneRefraction(), options };
    app.run();
}
