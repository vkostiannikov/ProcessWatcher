#include <ProcessList.h>
#include <Process.h>
#include <ProcessWatcher.h>
#include <algorithm>

ProcessList::ProcessList()
{
    sortOrder_.store('U');
    ProcessWatcherInstance.setSort(std::bind<void>(&ProcessList::sort, this));
}

ProcessList::~ProcessList()
{
    ProcessWatcherInstance.setSort(nullptr);
}

void ProcessList::addProcess(const PID &pid)
{
    if (!contains(pid))
    {
        std::shared_ptr<Process> process = std::make_shared<Process>(this);
        process->open(pid);
        std::lock_guard<std::mutex> lock(mtx_);
        processList_.emplace_back(process);
    }
}

void ProcessList::removeProcess(const PID &pid)
{
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = std::find_if(processList_.begin(), processList_.end(), [&pid](const std::shared_ptr<Process> &process)
                           { return process->getInfo().id == std::stoi(pid); });
    if (it != processList_.end())
    {
        processList_.erase(std::remove(processList_.begin(), processList_.end(), *it), processList_.end());
    }
}

bool ProcessList::contains(const PID &pid)
{
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = std::find_if(processList_.begin(), processList_.end(), [&pid](const std::shared_ptr<Process> &process)
                           { return process->getPID() == pid; });
    return it != processList_.end();
}

void ProcessList::clear()
{
    std::lock_guard<std::mutex> lock(mtx_);
    processList_.clear();
}

const std::shared_ptr<Process> ProcessList::getProcess(size_t index)
{
    std::lock_guard<std::mutex> lock(mtx_);
    return processList_[index];
}

const std::shared_ptr<Process> ProcessList::operator[](size_t index)
{
    std::lock_guard<std::mutex> lock(mtx_);
    return processList_[index];
}

size_t ProcessList::size() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return processList_.size();
}

bool ProcessList::empty() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return processList_.empty();
}

void ProcessList::setSort(char order)
{
    sortOrder_ = order;
}

char ProcessList::getSortOrder() const
{
    return sortOrder_;
}

void ProcessList::sort()
{
    std::lock_guard<std::mutex> lock(mtx_);

    for (size_t i = 1; i < processList_.size(); ++i)
    {
        std::shared_ptr<Process> key = processList_[i];
        int j = i - 1;
        while (j >= 0 && *processList_[j].get() < *key.get())
        {
            processList_[j + 1] = processList_[j];
            --j;
        }
        processList_[j + 1] = key;
    }
}