#pragma once
#include <chrono>

class Timer {
public:
    Timer() {
        startTime = std::chrono::high_resolution_clock::now();
        previousTime = startTime;
    }

    // Resets the timer
    void reset() {
        startTime = std::chrono::high_resolution_clock::now();
        previousTime = startTime;
    }

    // Returns the time in seconds since the timer started
    float elapsed() const {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = now - startTime;
        return duration.count();
    }

    // Returns the delta time since the last call to update()
    float update() {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = now - previousTime;
        previousTime = now;
        return delta.count();
    }

private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point previousTime;
};
