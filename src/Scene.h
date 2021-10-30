#pragma once
#include "Lights.h"
#include "Objects.h"
#include <vector>


// simple container for accessing the lights and objects in the order they were added
// currently only supports indexed access, no custom iterators (yet)
class Scene {
public:
    Scene() = default;

    void addLight(PointLight&& light);
    void addSceneObject(Sphere&& object);
    void addSceneObject(Triangle&& object);

    const ILight&  getLight(size_t index)  const;
    const IObject& getObject(size_t index) const;

    size_t getNumLights()  const;
    size_t getNumObjects() const;

private:
    std::vector<std::unique_ptr<ILight>>  lights_;
    std::vector<std::unique_ptr<IObject>> objects_;
};
std::ostream& operator<<(std::ostream& os, const Scene& scene);
