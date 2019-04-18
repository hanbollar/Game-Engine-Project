#include "timer.h"

Timer::Timer() : time_(0), running_(false)
{}

Timer::~Timer()
{}

void Timer::Tick() {
    time_ += 0.1f;
}

void Timer::Play() {
    running_ = true;
}

void Timer::Pause() {
    running_ = false;
}

bool Timer::IsRunning() {
    return running_;
}

float Timer::GetDuration(const float& previous_time) {
    Tick();
    return time_ - previous_time;
}

float Timer::GetTime() {
    Tick();
    return time_;
}