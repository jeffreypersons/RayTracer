#pragma once
#include "Lights.hpp"
#include "Objects.hpp"
#include <vector>


/*
Simple container for accessing the lights and objects in the order they were added.

Currently only supports indexed access, no custom iterators (yet).
Note that the scene takes full ownership over all of its data.
*/
class Scene {
public:
    Scene() = default;

    void addAmbientLight(Color&& ambientLightColor);
    void addLight(PointLight&& light);
    void addSceneObject(Sphere&& object);
    void addSceneObject(Triangle&& object);

    const Color& getAmbientLightColor() const;
    const ILight& getLight(size_t index) const;
    const IObject& getObject(size_t index) const;

    size_t getNumLights() const;
    size_t getNumObjects() const;

private:
    Color ambientLightColor_;
    std::vector<std::unique_ptr<ILight>> lights_;
    std::vector<std::unique_ptr<IObject>> objects_;
};
std::ostream& operator<<(std::ostream& os, const Scene& scene);
