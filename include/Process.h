#if !defined(__PROCESS_H__)
#define __PROCESS_H__

#include <string>
#include <memory>
#include <mutex>
#include <ISubscriber.h>

class ProcessList;

struct ProcessInfo
{
    int id;
    std::string status;
    double executionTime; // in seconds
    double ramUsage;      // in MB
    double cpuUsage;      // in percentage
    double oldCpuUsage;   // in percentage
    long totalCpuTime;
    long oldTotalCpuTime;
    std::string priority;
    std::string name;

    ProcessInfo() : id(0),
                    executionTime(0),
                    ramUsage(0),
                    cpuUsage(0),
                    oldCpuUsage(0),
                    totalCpuTime(0),
                    oldTotalCpuTime(0)
    {
    }
};

class Process : public ISubscriber
{
public:
    Process(ProcessList *info);
    ~Process();
    void open(const std::string &pid);
    void close();
    void update() override;
    const ProcessInfo &getInfo() const;
    const std::string &getPID() const;
    double getCpuUsage() const;
    double getRamUsage() const;
    const std::string getName() const;
    const std::string getStatus() const;
    const std::string getPriority() const;
    const std::string getExecTime() const;

    Process(Process &&);
    Process &operator=(Process &&);

    bool operator<(const Process &process);

private:
    void innerUpdate();

private:
    ProcessInfo info_;
    std::string pid_;
    ProcessList *processList_;
    mutable std::mutex mtx_;
};

#endif