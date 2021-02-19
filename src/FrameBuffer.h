#pragma once
#include "Math.hpp"
#include "Color.hpp"
#include <ostream>
#include <fstream>


class FrameBuffer {
private:
    const size_t width_;
    const size_t height_;
    const size_t bufferSize;
    const std::unique_ptr<Color[]> pixels;

public:
    FrameBuffer(size_t width, size_t height);
    FrameBuffer(const Vec2& dimensions);
    FrameBuffer() = delete;
    FrameBuffer(FrameBuffer&) = delete;
    FrameBuffer& operator=(FrameBuffer&) = delete;

    void writeToFile(const std::string& filename, float gammaCorrection=2.20f);

    void setPixel(size_t i, const Color& color) noexcept;
    void setPixel(size_t row, size_t col, const Color& color) noexcept;
    constexpr Color getPixel(size_t i) const noexcept;
    constexpr Color getPixel(size_t row, size_t col) const noexcept;

    constexpr size_t width() const noexcept;
    constexpr size_t height() const noexcept;
    constexpr size_t numPixels() const noexcept;
    constexpr float aspectRatio() const noexcept;
    float megaPixels() const;
};
std::ostream& operator<<(std::ostream& os, const FrameBuffer& frameBuffer);
