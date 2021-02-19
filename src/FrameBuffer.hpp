#pragma once
#include "Math.hpp"
#include "Color.hpp"
#include <ostream>
#include <fstream>
#include <vector>


class FrameBuffer {
private:
    const size_t width;
    const size_t height;
    const size_t numPixels;
    const float aspectRatio;
    const float megaPixels;
    const Color initialColor;
    std::unique_ptr<Color[]> pixels;

public:
    FrameBuffer(size_t width, size_t height) :
            width(width),
            height(height),
            numPixels(width * height),
            aspectRatio(width / static_cast<float>(height)),
            megaPixels(computeMegaPixels(width, height)),
            initialColor(initialColor),
            pixels(std::make_unique<Color[]>(numPixels)) {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("frame buffer must be have dimensions greater than zero");
        }
    }
    FrameBuffer(const Vec2& size) :
        FrameBuffer(static_cast<size_t>(size.x), static_cast<size_t>(size.y))
    {}
    FrameBuffer(FrameBuffer&) = delete;
    FrameBuffer& operator=(FrameBuffer&) = delete;

    // save framebuffer as an array of 256 rgb-colored pixels, written to file at given location
    // note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
    void writeToFile(const std::string& filename, float gammaCorrection=2.20f) {
        const float invGamma = 1.00f / gammaCorrection;
        std::ofstream ofs(filename + ".ppm", std::ios::out | std::ios::binary);
        ofs << "P6\n" << width << " " << height << "\n255\n";
        for (size_t row = 0; row < height; row++) {
            for (size_t col = 0; col < width; col++) {
                Color color = getPixel(height - 1 - row, col);
                ofs << static_cast<unsigned char>(static_cast<int>((Math::pow(color.r, invGamma) * 255) + 0.50f))
                    << static_cast<unsigned char>(static_cast<int>((Math::pow(color.g, invGamma) * 255) + 0.50f))
                    << static_cast<unsigned char>(static_cast<int>((Math::pow(color.b, invGamma) * 255) + 0.50f));
            }
        }
        ofs.close();
    }

    void setPixel(size_t i, const Color& intensity)               noexcept { pixels[i] = intensity;                 }
    void setPixel(size_t row, size_t col, const Color& intensity) noexcept { pixels[width * row + col] = intensity; }
    
    constexpr Color getPixel(size_t i)               const noexcept { return pixels[i];                 }
    constexpr Color getPixel(size_t row, size_t col) const noexcept { return pixels[width * row + col]; }
    constexpr size_t getWidth()                      const noexcept { return width;                     }
    constexpr size_t getHeight()                     const noexcept { return height;                    }
    constexpr float  getAspectRatio()                const noexcept { return aspectRatio;               }
    constexpr size_t getNumPixels()                  const noexcept { return numPixels;                 }
    constexpr float getMegaPixels()                  const noexcept { return megaPixels;                }
    constexpr Color getDefaultColor()                const noexcept { return initialColor;              }
    std::string getImageDescription() const {
        return std::to_string(width) + "X" + std::to_string(height) +
            "(" + std::to_string(getMegaPixels()) + "MP)";
    }

private:
    static float computeMegaPixels(size_t width, size_t height, size_t numDigits=2) {
        return Math::roundToNearestDigit((width * height) / 1000000.00f, numDigits);
    }
};
inline std::ostream& operator<<(std::ostream& os, const FrameBuffer& frameBuffer) {
    os << "FrameBuffer("
          << "initial-color:("   << frameBuffer.getDefaultColor()     << "),"
          << "image-dimensions:" << frameBuffer.getImageDescription() << ","
          << "num-elements:"     << frameBuffer.getNumPixels()        << ","
          << "aspect-ratio:"     << frameBuffer.getAspectRatio()
       << ")";
    return os;
}
