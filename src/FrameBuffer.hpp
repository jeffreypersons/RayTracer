#pragma once
#include "Math.hpp"
#include "Color.hpp"
#include <istream>
#include <ostream>
#include <fstream>
#include <assert.h>


// fast, header file defined, color buffer
class FrameBuffer {
private:
    const size_t width_;
    const size_t height_;
    const size_t bufferSize_;
    const std::unique_ptr<Color[]> pixels_;
    
public:
    FrameBuffer() = delete;
    FrameBuffer(const FrameBuffer& frameBuffer)
        : width_(frameBuffer.width_),
          height_(frameBuffer.height_),
          bufferSize_(frameBuffer.bufferSize_),
          pixels_(std::make_unique<Color[]>(frameBuffer.bufferSize_)) {
        for (size_t i = 0; i < frameBuffer.numPixels(); i++) {
            setPixel(i, frameBuffer.getPixel(i));
        }
    }

    FrameBuffer& operator=(FrameBuffer& frameBuffer) = delete;
    FrameBuffer(const Vec2& dimensions)
        : FrameBuffer(static_cast<size_t>(dimensions.x), static_cast<size_t>(dimensions.y))
    {}
    FrameBuffer(size_t width, size_t height)
        : width_     (width),
          height_    (height),
          bufferSize_(width * height),
          pixels_    (std::make_unique<Color[]>(bufferSize_)) {
        if (width <= 0 || height <= 0 || bufferSize_ <= 0) {
            throw std::invalid_argument("frame buffer must be have dimensions greater than zero");
        }
    }

    void setPixel(size_t i, const Color& color) noexcept {
        assert((i >= 0 && i < bufferSize_));
        pixels_[i] = color;
    }
    void setPixel(size_t row, size_t col, const Color& color) noexcept {
        assert((row >= 0 && row < height_) && (col >= 0 && col < width_));
        pixels_[width_ * row + col] = color;
    }

    constexpr Color getPixel(size_t i) const noexcept {
        assert((i >= 0 && i < bufferSize_));
        return pixels_[i];
    }
    constexpr Color getPixel(size_t row, size_t col) const noexcept {
        assert((row >= 0 && row < height_) && (col >= 0 && col < width_));
        return pixels_[width_ * row + col];
    }
    constexpr std::pair<size_t, size_t> getPixelRowCol(size_t i) const noexcept {
        assert((i >= 0 && i < bufferSize_));
        const size_t row = i / width_;
        const size_t col = i - (row * width_);
        return std::make_pair(row, col);
    }

    float megaPixels() const {
        return Math::roundToNearestDigit((bufferSize_) / 1000000.00f, 2);
    }

    constexpr size_t width()      const noexcept { return width_;      }
    constexpr size_t height()     const noexcept { return height_;     }
    constexpr size_t numPixels()  const noexcept { return bufferSize_; }
    constexpr float aspectRatio() const noexcept { return width_ / static_cast<float>(height_); }
};
inline std::ostream& operator<<(std::ostream& os, const FrameBuffer& frameBuffer) {
    os << "FrameBuffer("
         << "Dimensions{"
           << "width:" << frameBuffer.width() << ","
           << "height:" << frameBuffer.height() << ","
           << "pixel-count:" << frameBuffer.numPixels() << "}, "
         << "MetaData{"
           << "mega-pixels:" << frameBuffer.megaPixels() << ","
           << "aspect-ratio:" << frameBuffer.aspectRatio() << "}"
         << ")";
    return os;
}
