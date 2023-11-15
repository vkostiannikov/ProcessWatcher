#include <ProcessWatcher.h>
#include <Process.h>
#include <algorithm>
#include <constants.h>


ProcessWatcher::ProcessWatcher()
{
    stop_.store(false);
    thread_.emplace_back(std::thread(&ProcessWatcher::work_thread, this));
}

ProcessWatcher::~ProcessWatcher()
{
    stop_.store(true);
    cv_.notify_all();
    std::for_each(thread_.begin(), thread_.end(), [](std::thread& t) {
        if(t.joinable())
            t.join();
    });
}

void ProcessWatcher::subscribe(ISubscriber* process)
{
    subscribers_.add(process);
}

void ProcessWatcher::unsubscribe(ISubscriber* process)
{
    subscribers_.remove(process);
}

void ProcessWatcher::update()
{
    while (auto subscriber = subscribers_.getNext())
    {
        if(stop_.load())
            return;

        if(subscriber)
            subscriber->update();
    }
}

void ProcessWatcher::work_thread()
{
    while (!stop_.load())
    {
        subscribers_.resetNext();
        update();
        {
            std::unique_lock<std::mutex> lock(mtx_, std::defer_lock);
            if(lock.try_lock())
            {
                if(sort_)
                {
                    sort_();
                }
                cv_.notify_all();
            }
            else
            {
                cv_.wait(lock, [this](){ return true;});
            }
        }
            
        timer_.wait();
    }
}

void ProcessWatcher::setSort(std::function<void()> sort)
{
    std::unique_lock<std::mutex> lock(mtx_);
    sort_ = sort;
}