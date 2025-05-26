#include "frametimer.h"

FrameTimer::FrameTimer(double target)
    : m_last{std::chrono::high_resolution_clock::now()}
    , m_frameTarget{static_cast<int>(1000.0/target)}
{

}

void FrameTimer::clock() {
    m_last = std::chrono::high_resolution_clock::now();
}

void FrameTimer::sync() const {
    std::chrono::milliseconds elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - m_last
        );

    if (elapsed < m_frameTarget) {
        std::this_thread::sleep_for(m_frameTarget - elapsed);
    }
}
