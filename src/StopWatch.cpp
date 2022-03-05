#include "StopWatch.hpp"
#include <optional>
#include <omp.h>


StopWatch::StopWatch()
    : startTime_(std::nullopt),
      stopTime_ (std::nullopt) {}


void StopWatch::reset() {
    startTime_ = std::nullopt;
    stopTime_  = std::nullopt;
}

void StopWatch::start() {
    if (!isRunning()) {
        startTime_ = currentTime();
        stopTime_  = std::nullopt;
    }
}

void StopWatch::stop() {
    if (isRunning()) {
        stopTime_ = currentTime();
    }
}


bool StopWatch::isRunning() const {
    return startTime_.has_value() && !stopTime_.has_value();
}

bool StopWatch::isFinished() const {
    return startTime_.has_value() && stopTime_.has_value();
}

double StopWatch::elapsedTime() const {
    if (isRunning()) {
        return currentTime() - startTime_.value();
    }
    if (isFinished()) {
        return stopTime_.value() - startTime_.value();
    }
    return 0;
}


double StopWatch::currentTime() {
    return omp_get_wtime();
}
