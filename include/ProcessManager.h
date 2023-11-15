#if !defined(__PROCESS_MANAGER_H__)
#define __PROCESS_MANAGER_H__

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <IProcessReader.h>
#include <atomic>
#include <ProcessList.h>
#include <formatter.h>

class Display;
class ProcessReader;



class ProcessManager {

public:
    ProcessManager(std::shared_ptr<Display> display);
    ~ProcessManager();
    void start();
    void stop();
    void setSort(char order);

private:
    void workerThread();
    void checkProckDir();

private:
    ProcessList processList_;
    Formatter formatter_;
    std::shared_ptr<Display> display_;
    std::mutex mtx_;
    std::atomic_bool stop_;
    std::thread workerThread_;
};

#endif //__PROCESS_MANAGER_H__