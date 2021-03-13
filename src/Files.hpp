#pragma once
#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <exception>
#include <filesystem>
#include <assert.h>
#include <string_view>
#include "FrameBuffer.hpp"


namespace detail {
    inline bool pathExists(std::string_view path) {
        return std::filesystem::exists(path);
    }
    inline bool hasExtension(std::string_view path, std::string_view extension) {
        assert(extension[0] == '.');
        return std::filesystem::path(path).extension().string() == extension;
    }
    inline std::ifstream openPpmFileForReading(const std::string& filepath) {
        if (!hasExtension(filepath, ".ppm")) {
            throw std::runtime_error("Cannot read file `" + filepath + "` - does not end with .ppm");
        }
        if (!pathExists(filepath)) {
            throw std::runtime_error("Cannot read file `" + filepath + "` - does not exist");
        }

        std::ifstream ifs(filepath, std::ios::in | std::ios::binary);
        if (!ifs) {
            throw std::runtime_error("Cannot read file `" + filepath + "` - error while opening for write");
        }
        return ifs;
    }
    inline std::ofstream openPpmFileForWriting(const std::string& filepath) {
        if (!hasExtension(filepath, ".ppm")) {
            throw std::runtime_error("Cannot write file `" + filepath + "` - does not end with .ppm");
        }

        std::ofstream ofs(filepath, std::ios::out | std::ios::binary);
        if (!ofs) {
            throw std::runtime_error("Cannot write file `" + filepath + "` - error while opening for write");
        }
        return ofs;
    }
}

namespace Files {
    // save framebuffer as an array of 256 rgb-colored pixels_, written to file at given location
    // note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
    inline FrameBuffer readPpm(const std::string filepath) {
        std::ifstream ifs = detail::openPpmFileForReading(filepath);

        // read metadata from header, then skip ahead to binary color data
        std::string header;
        int bufferWidth, bufferHeight, numBytes;
        ifs >> header >> bufferWidth >> bufferHeight >> numBytes;
        if (header != "P6" || bufferWidth <= 0 || bufferHeight <= 0 || numBytes != 255) {
            throw std::runtime_error("File `" + filepath + ".ppm` has an invalid header - expected magic number (P6), " +
                                     "non-zero image dimensions, and byte size of 255");
        }
        ifs.ignore(256, '\n');

        FrameBuffer frameBuffer{ static_cast<size_t>(bufferWidth), static_cast<size_t>(bufferHeight) };
        unsigned char colorValues[3];
        for (int i = 0; i < frameBuffer.numPixels(); ++i) {
            ifs.read(reinterpret_cast<char*>(colorValues), 3);
            frameBuffer.setPixel(i, Color(colorValues[0], colorValues[1], colorValues[2]));
        }
        return frameBuffer;
    }

    // save framebuffer as an array of 256 rgb-colored pixels_, written to file at given location
    // note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
    inline void writePpm(const FrameBuffer& frameBuffer, const std::string filepath) {
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
    inline void writePpmWithGammaCorrection(const FrameBuffer& frameBuffer, const std::string filepath, float gammaCorrection=2.20f) {
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
