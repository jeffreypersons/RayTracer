#pragma once
#include "App.h"
#include "Text.hpp"
#include "Files.hpp"
#include <iostream>
#include <string>


std::ostream& operator<<(std::ostream& os, const AppOptions& appOptions) {
    os << "AppOptions("
         << "Output{"
           << "logInfo:" << appOptions.logInfo          << ","
           << "gamma:"   << appOptions.imageOutputGamma << ","
           << "file:\'"  << appOptions.imageOutputFile  << "\',"
           << "size:("    << appOptions.imageOutputSize << ")}, "
         << "RayTracing{"
           << "bias:"             << appOptions.rayTracingBias            << ","
           << "reflection-limit:" << appOptions.rayTracingReflectionLimit << ","
           << "sky-color:("       << appOptions.skyBoxColor               << "),"
           << "shadow-color:("    << appOptions.shadowColor               << ")}, "
         << "SceneViewing{"
           << "near-clip-z:"   << appOptions.cameraNearZ       << ","
           << "far-clip-z:"    << appOptions.cameraFarZ        << ","
           << "field-of-view:" << appOptions.cameraFieldOfView << ","
           << "view-target:("  << appOptions.viewTarget        << "),"
           << "view-offset:("  << appOptions.viewOffset        << ")}"
       << ")";
    return os;
}

App::App(Scene&& scene, const AppOptions& options)
    : options_(options),
      scene_(std::move(scene)),
      camera_(),
      rayTracer_(),
      frameBuffer_(options.imageOutputSize),
      stopWatch_() {
    if (options_.logInfo) {
        std::cout << Text::padSides(" Configuring App ", '*', 80) << "\n";
        std::cout << options_ << "\n\n";
        std::cout << *this;
    }
    rayTracer_.setBias(options.rayTracingBias);
    rayTracer_.setMaxNumReflections(options.rayTracingReflectionLimit);
    rayTracer_.setShadowColor(options.shadowColor);
    rayTracer_.setBackgroundColor(options.skyBoxColor);
    camera_.setNearClip(options.cameraNearZ);
    camera_.setFarClip(options.cameraFarZ);
    camera_.setAspectRatio(frameBuffer_.aspectRatio());
    camera_.setFieldOfView(options_.cameraFieldOfView);
    camera_.lookAtFrom(options_.viewTarget, options_.viewTarget + options_.viewOffset);
    if (options_.logInfo) {
        std::cout << Text::padSides(" Configuring App ", '*', 80) << "\n";
        std::cout << *this;
    }
}

void App::run() {
    if (options_.logInfo) {
        std::cout << "\n" << Text::padSides(" Tracing `" + options_.imageOutputFile + "` ", '*', 80) << "\n";

        std::cout << "Tracing started...";
        stopWatch_.start();
        rayTracer_.traceScene(camera_, scene_, frameBuffer_);
        stopWatch_.stop();
        std::cout << "finished in " << stopWatch_.elapsedTime() << " seconds" << "\n";

        Files::writePpmWithGammaCorrection(frameBuffer_, options_.imageOutputFile, options_.imageOutputGamma);
        std::cout << "Wrote to file \'"              << options_.imageOutputFile << "\' "
                  << "with gamma correction set to " << options_.imageOutputGamma << "\n\n";
    } else {
        rayTracer_.traceScene(camera_, scene_, frameBuffer_);
        Files::writePpmWithGammaCorrection(frameBuffer_, options_.imageOutputFile, options_.imageOutputGamma);
    }
}

inline std::ostream& operator<<(std::ostream& os, const App& app) {
    os << app.frameBuffer_ << "\n\n"
       << app.rayTracer_   << "\n\n"
       << app.scene_       << "\n\n"
       << app.camera_      << "\n";
    return os;
}