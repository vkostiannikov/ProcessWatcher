#if !defined(_PROCESS_LIST_H_)
#define _PROCESS_LIST_H_

#include <string>
#include <mutex>
#include <memory>
#include <atomic>
#include <vector>
#include <Process.h>

using PID = std::string;
using ProcessListArray = std::vector<std::shared_ptr<Process>>;

class ProcessReader;

class ProcessList
{
public:
    ProcessList();
    ~ProcessList();
    void addProcess(const PID& pid);
    void removeProcess(const PID& pid);
    bool contains(const PID& pid);
    void clear();
    void setSort(char order);
    void sort();
    char getSortOrder() const;
    const std::shared_ptr<Process> getProcess(size_t index);
    const std::shared_ptr<Process> operator[](size_t index);

    size_t size() const;
    bool empty() const;
private:
    ProcessListArray processList_;
    mutable std::mutex mtx_;
    std::atomic<char> sortOrder_;
};


#endif