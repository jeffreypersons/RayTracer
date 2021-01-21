#pragma once
#include <omp.h>
#include <optional>


class Timer {
private:
    std::optional<double> startTime;
    std::optional<double> stopTime;
    
    static std::optional<double> currentTime()       { return std::optional<double>(omp_get_wtime()); }
    static double duration(double start, double end) { return end - start;                            }
public:
    Timer() :
        startTime(std::nullopt),
        stopTime(std::nullopt)
    {}
    void clear() {
        startTime = std::nullopt;
        stopTime  = std::nullopt;
    }
    void start() {
        if (!isRunning()) {
            startTime = currentTime();
            stopTime  = std::nullopt;
        }
    }
    void stop() {
        if (isRunning()) {
            stopTime = currentTime();
        }
    }

    bool isRunning()  const { return startTime.has_value() && !stopTime.has_value(); }
    bool isFinished() const { return startTime.has_value() && stopTime.has_value();  }
    double getStartTime() const {
        if (!startTime.has_value()) {
            throw std::runtime_error("startTime undefined - requires call to start() after initialization or clear()");
        }
        return startTime.value();
    }
    double getStopTime() const {
        if (!stopTime.has_value()) {
            throw std::runtime_error("stopTime undefined - computed in stop() after calling start()");
        }
        return stopTime.value();
    }
    double elapsedTime() const {
        if (isRunning()) {
            return duration(startTime.value(), currentTime().value());
        }
        if (isFinished()) {
            return duration(startTime.value(), stopTime.value());
        }
        return 0;
    }
};
