#include "FrameBuffer.h"
#include "Math.hpp"
#include "Color.hpp"
#include <ostream>
#include <fstream>
#include <assert.h>


FrameBuffer::FrameBuffer(size_t width, size_t height)
    : width_(width),
      height_(height),
      bufferSize(width * height),
      pixels(std::make_unique<Color[]>(bufferSize)) {
    if (width <= 0 || height <= 0 || bufferSize <= 0) {
        throw std::invalid_argument("frame buffer must be have dimensions greater than zero");
    }
}
FrameBuffer::FrameBuffer(const Vec2& dimensions) :
    FrameBuffer(static_cast<size_t>(dimensions.x), static_cast<size_t>(dimensions.y)) 
{}

// save framebuffer as an array of 256 rgb-colored pixels, written to file at given location
// note that the buffer stores colors relative to top left corner, while ppm is relative to the bottom left
void FrameBuffer::writeToFile(const std::string& filename, float gammaCorrection) {
    const float invGamma = 1.00f / gammaCorrection;
    std::ofstream ofs(filename + ".ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width_ << " " << height_ << "\n255\n";
    for (size_t row = 0; row < height_; row++) {
        for (size_t col = 0; col < width_; col++) {
            Color color = getPixel(height_ - 1 - row, col);
            ofs << static_cast<unsigned char>(static_cast<int>((Math::pow(color.r, invGamma) * 255) + 0.50f))
                << static_cast<unsigned char>(static_cast<int>((Math::pow(color.g, invGamma) * 255) + 0.50f))
                << static_cast<unsigned char>(static_cast<int>((Math::pow(color.b, invGamma) * 255) + 0.50f));
        }
    }
    ofs.close();
}

void FrameBuffer::setPixel(size_t i, const Color& intensity) noexcept {
    assert((i >= 0 && i < bufferSize));
    pixels[i] = intensity;
}
void FrameBuffer::setPixel(size_t row, size_t col, const Color& color) noexcept {
    assert((row >= 0 && row < height_) && (col >= 0 && col < width_));
    pixels[width_ * row + col] = color;
}
constexpr Color FrameBuffer::getPixel(size_t i) const noexcept {
    assert((i >= 0 && i < bufferSize));
    return pixels[i];
}
constexpr Color FrameBuffer::getPixel(size_t row, size_t col) const noexcept {
    assert((row >= 0 && row < height_) && (col >= 0 && col < width_));
    return pixels[width_ * row + col];
}

constexpr size_t FrameBuffer::width() const noexcept {
    return width_;
}
constexpr size_t FrameBuffer::height() const noexcept {
    return height_;
}
constexpr size_t FrameBuffer::numPixels() const noexcept {
    return bufferSize;
}
constexpr float FrameBuffer::aspectRatio() const noexcept {
    return width_ / static_cast<float>(height_);
}
float FrameBuffer::megaPixels() const {
    return Math::roundToNearestDigit((bufferSize) / 1000000.00f, 2);
}

std::ostream& operator<<(std::ostream& os, const FrameBuffer& frameBuffer) {
    os << "FrameBuffer("
         << "Dimensions{"
           << "width:"       << frameBuffer.width()     << ","
           << "height:"      << frameBuffer.height()    << ","
           << "pixel-count:" << frameBuffer.numPixels() << "}, "
         << "MetaData{"
           << "mega-pixels:"  << frameBuffer.megaPixels()  << ","
           << "aspect-ratio:" << frameBuffer.aspectRatio() << "}"
       << ")";
    return os;
}
