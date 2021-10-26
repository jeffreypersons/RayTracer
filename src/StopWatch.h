#pragma once
#include <optional>


/*
Multiprocess compatible stopwatch for measuring wall clock time since last start.
*/
class StopWatch {
public:
    StopWatch();

    void reset();
    void start();
    void stop();
    bool isRunning() const;
    bool isFinished() const;
    double elapsedTime() const;

private:
    std::optional<double> startTime_;
    std::optional<double> stopTime_;

    static double currentTime();
};
