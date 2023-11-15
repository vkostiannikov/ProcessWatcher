#include <display.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>

Display::Display()
    : stop_(false)
{
    workerThread_ = std::thread(&Display::workerThread, this);
}

Display::~Display()
{
    stop();
    cv_.notify_one();
    if(workerThread_.joinable())
    {
        workerThread_.join();
    }
    clearScreen();
}

void Display::show(const std::string& str)
{
    //std::lock_guard<std::mutex> lock(mtx_);
    std::cout << str << std::endl;
}

void Display::post(const std::string& str)
{
    std::lock_guard<std::mutex> lock(mtx_);
    messages_.push_back(str);
    cv_.notify_one();
}

void Display::clearScreen()
{
    std::cout << "\033[2J\033[1;1H";
}

void Display::hideCursor()
{
    std::cout << "\033[?25l";
}

void Display::showCursor()
{
    std::cout << "\033[?25h";
}

void Display::workerThread()
{
    clearScreen();
    while(!stop_.load())
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this](){ 
            return !messages_.empty() || stop_.load();});
        if(!messages_.empty())
        {
            clearScreen();
            for(const auto& msg : messages_)
            {
                show(msg);
            }
            messages_.clear();
        }
    }
}

void Display::stop()
{
    stop_ = true;
}