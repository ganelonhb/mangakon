#ifndef __FRAMETIMER_H__
#define __FRAMETIMER_H__

#include <chrono>
#include <thread>

constexpr double DEFAULT_TARGET_FRAMERATE = 60.0;

class FrameTimer {
public:
    explicit FrameTimer(double target = DEFAULT_TARGET_FRAMERATE);

    void clock();
    void sync() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_last;
    const std::chrono::milliseconds m_frameTarget;
};

#endif
