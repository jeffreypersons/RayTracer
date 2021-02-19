#include "Scene.h"
#include "Lights.h"
#include "Objects.h"
#include <vector>


Scene::Scene()
    : lights(),
      objects()
{}

void Scene::addLight(PointLight light) {
    lights.push_back(std::move(std::make_unique<PointLight>(light)));
}
void Scene::addSceneObject(Sphere object) {
    objects.push_back(std::move(std::make_unique<Sphere>(object)));
}
void Scene::addSceneObject(Triangle object) {
    objects.push_back(std::move(std::make_unique<Triangle>(object)));
}

size_t Scene::getNumLights() const {
    return lights.size();
}
size_t Scene::getNumObjects() const {
    return objects.size();
}
const ILight& Scene::getLight(size_t index) const {
    return *lights[index].get();
}
const IObject& Scene::getObject(size_t index) const {
    return *objects[index].get();
}

std::ostream& operator<<(std::ostream& os, const Scene& scene) {
    os << "Scene(\n  lights:[";
    for (size_t i = 0; i < scene.getNumLights(); i++) {
        os << "\n    " << i << " -- " << scene.getLight(i);
    }
    os << "\n]\n";

    os << "  objects:[";
    for (size_t i = 0; i < scene.getNumObjects(); i++) {
        os << "\n    " << i << " -- " << scene.getObject(i);
    }
    os << "\n])";
    return os;
}
