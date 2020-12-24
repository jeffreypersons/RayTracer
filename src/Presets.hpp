#pragma once
#include "Math.hpp"
#include "Colors.hpp"


namespace PresetResolutions {
constexpr Vec2 SD_240p  = Vec2( 426,  240);
constexpr Vec2 SD_360p  = Vec2( 640,  360);
constexpr Vec2 SD_480p  = Vec2( 854,  480);
constexpr Vec2 HD_720p  = Vec2(1280,  720);
constexpr Vec2 HD_1080p = Vec2(1920, 1080);
constexpr Vec2 HD_2K    = Vec2(2560, 1440);
constexpr Vec2 HD_4K    = Vec2(3840, 2160);
constexpr Vec2 HD_8K    = Vec2(7680, 4320);
}

namespace PresetMaterials {
constexpr Material smoothBlue = Material(
    Color(0.00f, 0.00f, 0.75f),
    Color(0.00f, 0.00f, 0.75f),
    Color(0.00f, 0.00f, 0.50f),
    0.50f, 0.50f
);
constexpr Material shinyBlue = Material(
    Color(0.00f, 0.00f, 0.25f),
    Color(0.00f, 0.00f, 0.15f),
    Color(0.00f, 0.00f, 1.00f),
    0.95f, 0.05f
);
constexpr Material roughRed = Material(
    Color(0.50f, 0.01f, 0.05f),
    Color(0.50f, 0.10f, 0.05f),
    Color(1.00f, 0.01f, 0.05f),
    0.75f, 0.25f
);
constexpr Material reflectiveGreen = Material(
    Color(0.25f, 0.10f, 0.25f),
    Color(0.25f, 1.00f, 0.25f),
    Color(0.25f, 0.50f, 0.25f),
    0.10f, 0.90f
);
constexpr Material pureWhite = Material(
    Color(0.00f, 1.00f, 1.00f),
    Color(1.00f, 0.50f, 1.00f),
    Color(1.00f, 1.00f, 0.50f),
    1.00f, 0.00f
);
constexpr Material flatYellow = Material(
    Color(1.00f, 1.00f, 0.00f),
    Color(1.00f, 1.00f, 0.00f),
    Color(1.00f, 1.00f, 0.00f),
    1.00f, 0.00f
);
};
