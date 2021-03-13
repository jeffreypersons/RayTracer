#pragma once
#include <fstream>
#include <exception>
#include <filesystem>
#include <assert.h>
#include "FrameBuffer.h"


namespace detail {
    inline std::ifstream openPpmFileForReading(const std::string& filepath) {
        if (std::filesystem::path(filepath).extension() != ".ppm") {
            throw std::runtime_error("Cannot read file \'" + filepath + "\' - does not end with .ppm");
        }
        if (!std::filesystem::exists(filepath)) {
            throw std::runtime_error("Cannot read file \'" + filepath + "\' - does not exist");
        }

        std::ifstream ifs(filepath, std::ios::in | std::ios::binary);
        if (!ifs) {
            throw std::runtime_error("Cannot read file \'" + filepath + "\' - error while opening for write");
        }
        return ifs;
    }
    inline std::ofstream openPpmFileForWriting(const std::string& filepath) {
        if (std::filesystem::path(filepath).extension() != ".ppm") {
            throw std::runtime_error("Cannot write file \'" + filepath + "\' - does not end with .ppm");
        }

        std::ofstream ofs(filepath, std::ios::out | std::ios::binary);
        if (!ofs) {
            throw std::runtime_error("Cannot write file \'" + filepath + "\' - error while opening for write");
        }
        return ofs;
    }
}

namespace Files {
    inline bool pathExists(const std::string& filepath) {
        return std::filesystem::exists(filepath);
    }
    inline std::string fileName(const std::string& filepath) {
        return std::filesystem::path(filepath).filename().stem().string();
    }
    inline std::string fileExtension(const std::string filepath) {
        return std::filesystem::path(filepath).extension().string();
    }
    inline std::string absolutePath(const std::string& filepath) {
        return std::filesystem::canonical(filepath).string();
    }

    // save framebuffer as an array of 256 rgb-colored pixels_, written to file at given location
    // note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
    inline FrameBuffer readPpm(const std::string& filepath) {
        std::ifstream ifs = detail::openPpmFileForReading(filepath);

        // read metadata from header, then skip ahead to binary color data
        std::string header;
        int bufferWidth, bufferHeight, numBytes;
        ifs >> header >> bufferWidth >> bufferHeight >> numBytes;
        if (header != "P6" || bufferWidth <= 0 || bufferHeight <= 0 || numBytes != 255) {
            throw std::runtime_error("File \'" + filepath + "\' has an invalid header - expected magic number (P6), " +
                                     "non-zero image dimensions, and byte size of 255");
        }
        ifs.ignore(256, '\n');

        FrameBuffer frameBuffer{ static_cast<size_t>(bufferWidth), static_cast<size_t>(bufferHeight) };
        unsigned char colorValues[3];
        for (size_t i = 0; i < frameBuffer.numPixels(); i++) {
            ifs.read(reinterpret_cast<char*>(colorValues), 3);
            frameBuffer.setPixel(i, Color(colorValues[0], colorValues[1], colorValues[2]));
        }
        return frameBuffer;
    }

    // save framebuffer as an array of 256 rgb-colored pixels_, written to file at given location
    // note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
    inline void writePpm(const std::string& filepath, const FrameBuffer& frameBuffer) {
        std::ofstream ofs = detail::openPpmFileForWriting(filepath);
        ofs << "P6\n" << frameBuffer.width() << " " << frameBuffer.height() << "\n255\n";
        for (size_t i = 0; i < frameBuffer.numPixels(); i++) {
            const Color color = frameBuffer.getPixel(i);
            ofs << static_cast<unsigned char>(color.r * 255)
                << static_cast<unsigned char>(color.g * 255)
                << static_cast<unsigned char>(color.b * 255);
        }
    }
    // save framebuffer as an array of 256 rgb-colored pixels_, written to file at given location
    // note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
    inline void writePpmWithGammaCorrection(const std::string& filepath, const FrameBuffer& frameBuffer,
                                            float gammaCorrection=2.20f) {
        std::ofstream ofs = detail::openPpmFileForWriting(filepath);
        const float invGamma = 1.00f / gammaCorrection;
        ofs << "P6\n" << frameBuffer.width() << " " << frameBuffer.height() << "\n255\n";
        for (size_t i = 0; i < frameBuffer.numPixels(); i++) {
            const Color color = frameBuffer.getPixel(i);
            ofs << static_cast<unsigned char>((Math::pow(color.r, invGamma) * 255) + 0.50f)
                << static_cast<unsigned char>((Math::pow(color.g, invGamma) * 255) + 0.50f)
                << static_cast<unsigned char>((Math::pow(color.b, invGamma) * 255) + 0.50f);
        }
    }
}
