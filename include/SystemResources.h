#if !defined(__SYSTEM_RESOURCES_H__)
#define __SYSTEM_RESOURCES_H__

#include <ISubscriber.h>
#include <Singleton.h>

#include <string>
#include <atomic>

struct CpuTime {
    long user, nice, system, idle, iowait, irq, softirq, steal;
    CpuTime() : user(0), nice(0), system(0), idle(0), iowait(0), irq(0), softirq(0), steal(0) {}
    // Add other fields if needed

    long Total() const { return user + nice + system + idle + iowait + irq + softirq + steal; }
    long Work() const { return user + nice + system + irq + softirq + steal; }
};

struct MemInfo
{
    long MemTotal, MemFree, Buffers, Cached;
};

class SystemResources : public ISubscriber
{
public:
    SystemResources();
    ~SystemResources();
    unsigned long long totalRam();
    unsigned long long freeRam();
    unsigned long long usedRam();
    unsigned long long cachedRam();
    double ramUsage();
    double cpuLoad();
    long totalCpuTime();
    void update() override;

private:
    void innerUpdate();
    void updateRam();
    void updateCpu();
    CpuTime getCputime();
    MemInfo getMeminfo();

private:
    MemInfo memInfo_;
    std::atomic<double> cpuLoad_;
    CpuTime cpuTime_;
    std::atomic<long> totalCpuTime_;
};

#define SystemResourcesInstance Singleton<SystemResources>::getInstance()

#endif // __SYSTEM_RESOURCES_H__