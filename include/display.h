#if !defined(__DISPLAY_H__)
#define __DISPLAY_H__

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <atomic>
#include <ProcessManager.h>

class Display {
public:
    Display();
    ~Display();

    void post(const std::string& str);
    void stop();

private:
    void show(const std::string& str);
    void workerThread();
    //std::string Format(const std::vector<ThreadInfo>& threadsInfo);
    void clearScreen();
    void hideCursor();
    void showCursor();
private:
    std::mutex mtx_;
    std::deque<std::string> messages_;
    std::atomic<bool> stop_;
    std::condition_variable cv_;
    std::thread workerThread_;
};

#endif // __DISPLAY_H__