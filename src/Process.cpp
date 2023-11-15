
#include <iostream>
#include <unistd.h>
#include "Process.h"
#include "ProcessList.h"
#include "ProcessWatcher.h"
#include "constants.h"
#include "RawProcess.h"
#include "SystemResources.h"

Process::Process(ProcessList *psList)
{
    try
    {
        ProcessWatcherInstance.subscribe(this);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    if (psList)
    {
        processList_ = psList;
    }
}

Process::Process(Process &&process)
{
    pid_ = process.pid_;
    info_ = process.info_;
    processList_ = process.processList_;
    process.processList_ = nullptr;
}

Process &Process::operator=(Process &&process)
{
    if (this != &process)
    {
        pid_ = process.pid_;
        info_ = process.info_;
        processList_ = process.processList_;
        process.processList_ = nullptr;
    }
    return *this;
}

Process::~Process()
{
    close();
}

void Process::open(const std::string &pid)
{
    pid_ = pid;
    innerUpdate();
}

void Process::close()
{
    if (processList_)
    {
        processList_->removeProcess(pid_);
    }
    std::lock_guard<std::mutex> lock(mtx_);
    try
    {
        ProcessWatcherInstance.unsubscribe(this);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Process::update()
{
    innerUpdate();
}

void Process::innerUpdate()
{
    const std::string path = Constants::PATH_TO_PROCESS_LIST + pid_ + "/stat";
    RawProcess rawProcess(path);
    try
    {
        rawProcess.read();
        std::lock_guard<std::mutex> lock(mtx_);

        info_.oldCpuUsage = info_.cpuUsage;
        info_.oldTotalCpuTime = info_.oldTotalCpuTime;
        info_.cpuUsage = rawProcess.cpuUsage();
        info_.name = rawProcess.name();
        info_.priority = rawProcess.priority();
        info_.ramUsage = rawProcess.ramUsage();
        info_.status = rawProcess.status();
        info_.totalCpuTime = SystemResourcesInstance.totalCpuTime();
    }
    catch (const std::exception &e)
    {
        close();
    }
}

const ProcessInfo &Process::getInfo() const
{
    return info_;
}

const std::string &Process::getPID() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return pid_;
}

double Process::getCpuUsage() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    int cpuUsage = info_.cpuUsage - info_.oldCpuUsage;
    if (info_.oldCpuUsage == 0)
    {
        return 0;
    }
    if (100 < cpuUsage)
    {
        cpuUsage = 100;
    }

    return (static_cast<double>(cpuUsage));
}

double Process::getRamUsage() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return static_cast<double>(info_.ramUsage * 4) / SystemResourcesInstance.totalRam() * 100;
}

const std::string Process::getName() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return info_.name;
}

const std::string Process::getStatus() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return info_.status;
}

const std::string Process::getPriority() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return info_.priority;
}

const std::string Process::getExecTime() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    long total_seconds = info_.cpuUsage;
    long days, hours, minutes, seconds;
    days = total_seconds / (24 * 3600);
    total_seconds = total_seconds % (24 * 3600);
    hours = total_seconds / 3600;
    total_seconds %= 3600;
    minutes = total_seconds / 60;
    seconds = total_seconds % 60;
    std::string t = std::to_string(days) + "d "
                     + (hours < 10 ? "0" : "") 
                     + std::to_string(hours) 
                     + ":" + (minutes < 10 ? "0" : "") 
                     + std::to_string(minutes) + ":" 
                     + (seconds < 10 ? "0" : "") 
                     + std::to_string(seconds);

    return t;
}

bool Process::operator<(const Process &process)
{
    switch (processList_->getSortOrder())
    {
    case 'U':
        return getCpuUsage() < process.getCpuUsage();
    case 'u':
        return getCpuUsage() > process.getCpuUsage();
    case 'M':
        return getRamUsage() < process.getRamUsage();
    case 'm':
        return getRamUsage() > process.getRamUsage();
    case 'P':
        return std::stoi(getPID()) < std::stoi(process.getPID());
    case 'p':
        return std::stoi(getPID()) > std::stoi(process.getPID());
    case 'C':
        return getPriority() < process.getPriority();
    case 'c':
        return getPriority() > process.getPriority();
    case 'N':
        return getName() < process.getName();
    case 'n':
        return getName() > process.getName();
    case 'T':
        return getStatus() < process.getStatus();
    case 't':
        return getStatus() > process.getStatus();
    default:
        return false;
    }
    return false;
}