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
    Color* pixels;

    float computeMegaPixels(size_t width, size_t height, size_t numDigits=2) {
        return Math::roundToNearestDigit((width * height) / 1000000.00f, numDigits);
    }

public:
    FrameBuffer(size_t width, size_t height, const Color& initialColor) :
            width(width),
            height(height),
            numPixels(width * height),
            aspectRatio(width / static_cast<float>(height)),
            megaPixels(computeMegaPixels(width, height)),
            initialColor(initialColor),
            pixels(nullptr) {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("frame buffer must be have dimensions greater than zero");
        }
        pixels = new Color[numPixels];
        for (int i = 0; i < numPixels; i++) {
            pixels[i] = this->initialColor;
        }
    }
    FrameBuffer(const Vec2& size, const Color& initialColor) :
        FrameBuffer(static_cast<size_t>(size.x), static_cast<size_t>(size.y), initialColor)
    {}

    FrameBuffer(const FrameBuffer& frameBuffer) :
            width(width),
            height(height),
            numPixels(width * height),
            aspectRatio(width / static_cast<float>(height)),
            megaPixels(computeMegaPixels(width, height)),
            initialColor(initialColor),
            pixels(nullptr) {
        pixels = new Color[numPixels];
        memcpy(pixels, frameBuffer.pixels, sizeof(Color) * width * height);
    }

    ~FrameBuffer() {
        if (pixels != nullptr) {
            delete[] pixels;
        }
    }

    // save framebuffer as an array of 256 rgb-colored pixels, written to file at given location
    // note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
    void writeToFile(const std::string& filename) {
        std::ofstream ofs(filename + ".ppm", std::ios::out | std::ios::binary);
        ofs << "P6\n" << width << " " << height << "\n255\n";
        for (size_t row = 0; row < height; row++) {
            for (size_t col = 0; col < width; col++) {
                Color color = getPixel(height - 1 - row, col);
                ofs << static_cast<unsigned char>(color.r * 255)
                    << static_cast<unsigned char>(color.g * 255)
                    << static_cast<unsigned char>(color.b * 255);
            }
        }
        ofs.close();
    }

    void setPixel(size_t i, const Color& color)               noexcept { pixels[i] = color;                 }
    void setPixel(size_t row, size_t col, const Color& color) noexcept { pixels[width * row + col] = color; }
    
    constexpr Color getPixel(size_t i)               const noexcept { return pixels[i];                 }
    constexpr Color getPixel(size_t row, size_t col) const noexcept { return pixels[width * row + col]; }
    constexpr size_t getWidth()                      const noexcept { return width;                     }
    constexpr size_t getHeight()                     const noexcept { return height;                    }
    constexpr float  getAspectRatio()                const noexcept { return aspectRatio;               }
    constexpr size_t getNumPixels()                  const noexcept { return numPixels;                 }
    constexpr float getMegaPixels()                  const noexcept { return megaPixels;                }
    constexpr Color getDefaultColor()                const noexcept { return initialColor;              }
    std::string getImageDescription() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2)
             << width << "X" << height
             << "(" << getMegaPixels() << "MP)";
        return ss.str();
    }
};
inline std::ostream& operator<<(std::ostream& os, const FrameBuffer& frameBuffer) {
    os << std::fixed << std::setprecision(3)
       << "FrameBuffer("
          << "initial-color:("   << frameBuffer.getDefaultColor()     << "),"
          << "image-dimensions:" << frameBuffer.getImageDescription() << ", "
          << "num-elements:"     << frameBuffer.getNumPixels()        << ","
          << "aspect-ratio:"     << frameBuffer.getAspectRatio()
       << ")";
    return os;
}
