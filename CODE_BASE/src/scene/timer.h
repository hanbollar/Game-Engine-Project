#pragma once

#include "globals.h"

#include <time.h>

class Timer {
private:
    bool running_;
    float time_;
public:
    Timer();
    ~Timer();

    void Tick();
    void Play();
    void Pause();

    bool IsRunning();

    float GetDuration(const float& previous_time);
    float GetTime();
};
