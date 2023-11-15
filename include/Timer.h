#if !defined(_TIMER_H_)
#define _TIMER_H_

#include <mutex>
#include <condition_variable>
#include <atomic>

class Timer
{
public:
    Timer();
    ~Timer();

    void setTimer(int seconds);
    void stop();
    void wait();

private:
    std::atomic_int time_to_wait_seconds_;
    std::mutex mtx_;
    std::condition_variable cv_;
};

#endif // _TIMER_H_