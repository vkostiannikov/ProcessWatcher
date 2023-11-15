#include "SystemResources.h"

#include <sys/sysinfo.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <ProcessWatcher.h>

SystemResources::SystemResources()
{
    innerUpdate();
    ProcessWatcherInstance.subscribe(this);
}

SystemResources::~SystemResources()
{
    ProcessWatcherInstance.unsubscribe(this);
}

unsigned long long SystemResources::totalRam()
{
    return memInfo_.MemTotal;
}

unsigned long long SystemResources::freeRam()
{
    return memInfo_.MemFree;
}

unsigned long long SystemResources::cachedRam()
{
    return memInfo_.Cached;
}

unsigned long long SystemResources::usedRam()
{
    return memInfo_.MemTotal - memInfo_.MemFree - memInfo_.Cached - memInfo_.Buffers;
}

double SystemResources::ramUsage()
{
    return static_cast<double>(usedRam()) / static_cast<double>(memInfo_.MemTotal) * 100;
}

double SystemResources::cpuLoad()
{
    return cpuLoad_;
}

void SystemResources::update()
{
    innerUpdate();
}

void SystemResources::innerUpdate()
{
    updateCpu();
    updateRam();
}

void SystemResources::updateRam()
{
    memInfo_ = getMeminfo();
}

void SystemResources::updateCpu()
{
    CpuTime current = getCputime();
    CpuTime previous = cpuTime_;
    long total = current.Total() - previous.Total();
    long work = current.Work() - previous.Work();
    if(work > 0 && total > 0)
    {
        cpuLoad_ = static_cast<double>(work) / static_cast<double>(total) * 100;
    }
    else
    {
        cpuLoad_ = 0;
    }
    cpuTime_ = current;
}

CpuTime SystemResources::getCputime()
{
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    file.close();

    CpuTime time;
    std::sscanf(line.c_str(), "cpu %ld %ld %ld %ld %ld %ld %ld %ld", &time.user, &time.nice, &time.system, &time.idle, &time.iowait, &time.irq, &time.softirq, &time.steal);
    return time;
}

long SystemResources::totalCpuTime()
{
    CpuTime cpuTime = getCputime();
    return  cpuTime.Total();
}

MemInfo SystemResources::getMeminfo()
{
    std::ifstream file("/proc/meminfo");

    MemInfo info;
    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 8) == "MemTotal") {
            std::sscanf(line.c_str(), "MemTotal: %ld kB", &info.MemTotal);
        } else if (line.substr(0, 7) == "MemFree") {
            std::sscanf(line.c_str(), "MemFree: %ld kB", &info.MemFree);
        } else if (line.substr(0, 7) == "Buffers") {
            std::sscanf(line.c_str(), "Buffers: %ld kB", &info.Buffers);
        } else if (line.substr(0, 6) == "Cached") {
            std::sscanf(line.c_str(), "Cached: %ld kB", &info.Cached);
        }
    }
    file.close();
    return info;
}