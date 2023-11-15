#include <Timer.h>

Timer::Timer()
    : time_to_wait_seconds_(1)
{
}

Timer::~Timer()
{
    stop();
}

void Timer::setTimer(int seconds)
{
    time_to_wait_seconds_ = seconds;
}

void Timer::stop()
{
    cv_.notify_all();
}

void Timer::wait()
{
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait_until(lock,
         std::chrono::system_clock::now() + std::chrono::seconds(time_to_wait_seconds_.load(std::memory_order_relaxed)));
}