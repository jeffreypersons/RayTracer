#pragma once
#include "Math.hpp"
#include "Color.hpp"
#include <istream>
#include <ostream>
#include <fstream>
#include <assert.h>


namespace CommonResolutions {
    inline constexpr Vec2 SD_240p  = Vec2(  426,  240);
    inline constexpr Vec2 SD_360p  = Vec2(  640,  360);
    inline constexpr Vec2 SD_480p  = Vec2(  854,  480);
    inline constexpr Vec2 HD_720p  = Vec2( 1280,  720);
    inline constexpr Vec2 HD_1080p = Vec2( 1920, 1080);
    inline constexpr Vec2 HD_2K    = Vec2( 2560, 1440);
    inline constexpr Vec2 HD_4K    = Vec2( 3840, 2160);
    inline constexpr Vec2 HD_5K    = Vec2( 5120, 2880);
    inline constexpr Vec2 HD_8K    = Vec2( 7680, 4320);
    inline constexpr Vec2 HD_10K   = Vec2(10328, 7760);
    inline constexpr Vec2 HD_12K   = Vec2(12288, 6480);
}

class FrameBuffer {
private:
    size_t width_;
    size_t height_;
    size_t bufferSize_;
    std::unique_ptr<Color[]> pixels_;
    
public:
    FrameBuffer()                         = delete;
    FrameBuffer(const FrameBuffer&)       = delete;
    FrameBuffer& operator=(FrameBuffer&)  = delete;
    FrameBuffer& operator=(FrameBuffer&&) = default;
    FrameBuffer(FrameBuffer&&)            = default;

    explicit FrameBuffer(Vec2 dimensions)
        : FrameBuffer(static_cast<size_t>(dimensions.x), static_cast<size_t>(dimensions.y))
    {}
    FrameBuffer(size_t width, size_t height)
        : width_     (width),
          height_    (height),
          bufferSize_(width * height),
          pixels_    (std::make_unique<Color[]>(bufferSize_)) {
        if (width <= 0 || height <= 0 || bufferSize_ <= 0) {
            throw std::invalid_argument("frame buffer must have dimensions greater than zero");
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
        return Math::roundToNearestDigit(bufferSize_ / 1000000.00f, 2);
    }

    constexpr size_t width()      const noexcept { return width_;                               }
    constexpr size_t height()     const noexcept { return height_;                              }
    constexpr size_t numPixels()  const noexcept { return bufferSize_;                          }
    constexpr float aspectRatio() const noexcept { return width_ / static_cast<float>(height_); }
};
inline std::ostream& operator<<(std::ostream& os, const FrameBuffer& frameBuffer) {
    os << "FrameBuffer("
         << "Size{"
           << "width:"       << frameBuffer.width()     << ","
           << "height:"      << frameBuffer.height()    << ","
           << "pixel-count:" << frameBuffer.numPixels() << "}, "
         << "MetaData{"
           << "mega-pixels:"  << frameBuffer.megaPixels()  << ","
           << "aspect-ratio:" << frameBuffer.aspectRatio() << "}"
         << ")";
    return os;
}
