#pragma once
#include <omp.h>
#include <optional>


class StopWatch {
private:
    std::optional<double> _startTime;
    std::optional<double> _stopTime;
    
    static double getCurrentTime() { return omp_get_wtime(); }
    static double duration(double start, double end) { return end - start; }
public:
    StopWatch() :
        _startTime(std::nullopt),
        _stopTime(std::nullopt)
    {}
    void reset() {
        _startTime = std::nullopt;
        _stopTime  = std::nullopt;
    }
    void start() {
        if (!isRunning()) {
            _startTime = getCurrentTime();
            _stopTime  = std::nullopt;
        }
    }
    void stop() {
        if (isRunning()) {
            _stopTime = getCurrentTime();
        }
    }

    bool isRunning() const {
        return _startTime.has_value() && !_stopTime.has_value();
    }
    bool isFinished() const {
        return _startTime.has_value() && _stopTime.has_value();
    }
    double elapsedTime() const {
        if (isRunning()) {
            return getCurrentTime() - _startTime.value();
        }
        if (isFinished()) {
            return _stopTime.value() - _startTime.value();
        }
        return 0;
    }
};
