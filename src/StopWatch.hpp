#pragma once
#include <omp.h>
#include <optional>


class StopWatch {
public:
    StopWatch()
        : startTime_(std::nullopt),
          stopTime_ (std::nullopt) {}

    void reset() {
        startTime_ = std::nullopt;
        stopTime_  = std::nullopt;
    }
    void start() {
        if (!isRunning()) {
            startTime_ = currentTime();
            stopTime_ = std::nullopt;
        }
    }
    void stop() {
        if (isRunning()) {
            stopTime_ = currentTime();
        }
    }

    bool isRunning() const {
        return startTime_.has_value() && !stopTime_.has_value();
    }
    bool isFinished() const {
        return startTime_.has_value() && stopTime_.has_value();
    }

    // compute time since start and now (or since time last stopped)
    double elapsedTime() const {
        if (isRunning()) {
            return currentTime() - startTime_.value();
        }
        if (isFinished()) {
            return stopTime_.value() - startTime_.value();
        }
        return 0;
    }

private:
    std::optional<double> startTime_;
    std::optional<double> stopTime_;

    static double currentTime() {
        return omp_get_wtime();
    }
};
