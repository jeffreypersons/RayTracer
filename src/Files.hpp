#pragma once
#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <exception>
#include <filesystem>
#include <assert.h>
#include "FrameBuffer.hpp"


namespace {
// read framebuffer as an array of 256 rgb-colored pixels, written to file at given location
// note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
Vec2 extractPpmImageSizeFromHeader(const std::string& filename, size_t& width, size_t& height) {
    std::ifstream ifs(filename + ".ppm", std::ios::in | std::ios::binary);
    if (!std::filesystem::exists(filename + ".ppm")) {
        throw std::runtime_error("File `" + filename + ".ppm` does not exist");
    }
    if (!ifs) {
        throw std::runtime_error("File `" + filename + ".ppm` cannot be opened");
    }

    std::string header;
    int bufferWidth, bufferHeight, numBytes;
    ifs >> header >> bufferWidth >> bufferHeight >> numBytes;
    if (header != "P6" || bufferWidth <= 0 || bufferHeight <= 0 || numBytes != 255) {
        throw std::runtime_error("File `" + filename + ".ppm` has an invalid header - expected magic number (P6), " +
                                 "non-zero image dimensions, and byte size of 255");
    }
    width  = static_cast<size_t>(bufferWidth);
    height = static_cast<size_t>(bufferHeight);
}
}

namespace Files {
// save framebuffer as an array of 256 rgb-colored pixels_, written to file at given location
// note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
void writePpm(const FrameBuffer& frameBuffer, const std::string& filename, float gammaCorrection=2.20f) {
    const float invGamma = 1.00f / gammaCorrection;
    std::ofstream ofs(filename + ".ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << frameBuffer.width() << " " << frameBuffer.height() << "\n255\n";
    for (size_t i = 0; i < frameBuffer.numPixels(); i++) {
        const Color color = frameBuffer.getPixel(i);
        ofs << static_cast<unsigned char>(static_cast<int>((Math::pow(color.r, invGamma) * 255) + 0.50f))
            << static_cast<unsigned char>(static_cast<int>((Math::pow(color.g, invGamma) * 255) + 0.50f))
            << static_cast<unsigned char>(static_cast<int>((Math::pow(color.b, invGamma) * 255) + 0.50f));
    }
    ofs.close();
}
}
