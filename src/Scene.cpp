#include "Scene.hpp"
#include "Lights.hpp"
#include "Objects.hpp"
#include <vector>
#include <assert.h>


void Scene::addLight(PointLight&& light) {
    lights_.push_back(std::make_unique<PointLight>(std::move(light)));
}
void Scene::addSceneObject(Sphere&& object) {
    objects_.push_back(std::make_unique<Sphere>(std::move(object)));
}
void Scene::addSceneObject(Triangle&& object) {
    objects_.push_back(std::make_unique<Triangle>(std::move(object)));
}

const ILight& Scene::getLight(size_t index) const {
    assert(index >= 0 && index < lights_.size());
    return *lights_[index];
}
const IObject& Scene::getObject(size_t index) const {
    assert(index >= 0 && index < objects_.size());
    return *objects_[index];
}
size_t Scene::getNumLights() const {
    return lights_.size();
}
size_t Scene::getNumObjects() const {
    return objects_.size();
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
