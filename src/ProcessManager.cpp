#include "ProcessManager.h"
#include <iostream>
#include <string>
#include "display.h"

#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "constants.h"
#include <filesystem>
#include "Timer.h"

ProcessManager::ProcessManager(std::shared_ptr<Display> display) : display_(display), stop_(false)
{
}

ProcessManager::~ProcessManager()
{
    stop_.store(true);
    if (workerThread_.joinable())
    {
        workerThread_.join();
    }
}

void ProcessManager::start()
{
    checkProckDir();
    workerThread_ = std::thread(&ProcessManager::workerThread, this);
}

void ProcessManager::stop()
{
    stop_.store(true);
}

void ProcessManager::workerThread()
{
    Timer timer;
    while (!stop_.load())
    {
        const auto data = formatter_.Format(processList_);
        display_->post(data);
        checkProckDir();
        timer.wait();
    }
    display_->stop();
}

void ProcessManager::checkProckDir()
{
    std::string path = Constants::PATH_TO_PROCESS_LIST;
    std::vector<std::string> pids;
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        if(entry.is_directory())
        {
            std::string path = entry.path();
            std::string pid = path.substr(path.find_last_of('/') + 1);
            if (pid.find_first_not_of("0123456789") == std::string::npos)
            {
                processList_.addProcess(pid);
            }
        }
    }
}

void ProcessManager::setSort(char order)
{
    processList_.setSort(order);
}