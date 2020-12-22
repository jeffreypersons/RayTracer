#pragma once
#include "Math.hpp"
#include "Colors.hpp"
#include "Cameras.hpp"
#include "Lights.hpp"
#include "SceneObjects.hpp"
#include "FrameBuffers.hpp"


namespace Presets {
const Material smoothBlue = Material(
    Color(0.00f, 0.00f, 0.75f),
    Color(0.00f, 0.00f, 0.75f),
    Color(0.00f, 0.00f, 0.50f),
    0.50f, 0.50f
);
const Material shinyBlue = Material(
    Color(0.00f, 0.00f, 0.25f),
    Color(0.00f, 0.00f, 0.15f),
    Color(0.00f, 0.00f, 1.00f),
    0.95f, 0.05f
);
const Material roughRed = Material(
    Color(0.50f, 0.01f, 0.05f),
    Color(0.50f, 0.10f, 0.05f),
    Color(1.00f, 0.01f, 0.05f),
    0.75f, 0.25f
);
const Material reflectiveGreen = Material(
    Color(0.25f, 0.10f, 0.25f),
    Color(0.25f, 1.00f, 0.25f),
    Color(0.25f, 0.50f, 0.25f),
    0.10f, 0.90f
);
const Material pureWhite = Material(
    Color(0.00f, 1.00f, 1.00f),
    Color(1.00f, 0.50f, 1.00f),
    Color(1.00f, 1.00f, 0.50f),
    1.00f, 0.00f
);
const Material flatYellow = Material(
    Color(1.00f, 1.00f, 0.00f),
    Color(1.00f, 1.00f, 0.00f),
    Color(1.00f, 1.00f, 0.00f),
    1.00f, 0.00f
);
};

// todo: add scene class
class Tracer {
private:
    const std::string OUTPUT_FILE = "./scene";
    static constexpr Vec2 IMAGE_SIZE{ 1500, 1500 };
    static constexpr Color BACKGROUND_COLOR{ 0.25f, 0.25f, 0.25f };
    static constexpr size_t MAX_NUM_REFLECTIONS = 5;

    // output targets
    std::string outputFile;

    // core data
    std::unique_ptr<FrameBuffer> frameBuffer;

    // camera viewing and rendering
    std::shared_ptr<ViewPlane> viewPlane;
    std::unique_ptr<RenderCam> renderCam;

    // lights and objects in the scene
    std::vector<Light> sceneLights;
    std::vector<std::unique_ptr<ISceneObject>> sceneObjects;

public:

    Tracer();
    void trace() {
        
    }
    void setup();
    void draw() const;
    Color traceRay(const Ray&, size_t) const;
};
