#include "Scene.h"
#include "Lights.h"
#include "Objects.h"
#include <vector>
#include <assert.h>


void Scene::addLight(PointLight&& light) {
    lights.push_back(std::make_unique<PointLight>(std::move(light)));
}
void Scene::addSceneObject(Sphere&& object) {
    objects.push_back(std::make_unique<Sphere>(std::move(object)));
}
void Scene::addSceneObject(Triangle&& object) {
    objects.push_back(std::make_unique<Triangle>(std::move(object)));
}

size_t Scene::getNumLights() const {
    return lights.size();
}
size_t Scene::getNumObjects() const {
    return objects.size();
}
const ILight& Scene::getLight(size_t index) const {
    assert(index >= 0 && index < lights.size());
    return *lights[index];
}
const IObject& Scene::getObject(size_t index) const {
    assert(index >= 0 && index < objects.size());
    return *objects[index];
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
