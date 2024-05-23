#if !defined(_PROCESS_WATCHER_H_)
#define _PROCESS_WATCHER_H_

#include <vector>
#include <shared_mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <functional>
#include <Singleton.h>
#include <ISubscriber.h>
#include <Timer.h>
#include <SubscribersList.h>


/**
 * @brief The ProcessWatcher class monitors a list of processes and updates subscribers with given interval.
 * 
 */
class ProcessWatcher
{
public:
    /**
     * @brief Construct a new ProcessWatcher object.
     * 
     */
    ProcessWatcher();

    /**
     * @brief Destroy the ProcessWatcher object.
     * 
     */
    ~ProcessWatcher();

    /**
     * @brief Subscribe to updates for a given process.
     * 
     * @param process The process to subscribe to.
     */
    void subscribe(ISubscriber* process);

    /**
     * @brief Unsubscribe from updates for a given process.
     * 
     * @param process The process to unsubscribe from.
     */
    void unsubscribe(ISubscriber* process);

    /**
     * @brief Updates subscribers.
     * 
     */
    void update();

    void setSort(std::function<void()> sort);

    void start();
    void stop();

private:
    /**
     * @brief The work thread that updates.
     * 
     */
    void work_thread();

private:
    std::atomic_bool stop_; /**< A flag indicating whether the work thread should stop. */
    SubscribersList<ISubscriber*> subscribers_; /**< The list of processes to update. */
    mutable std::mutex mtx_; /**< A mutex to protect the list of processes. */
    std::vector<std::thread> thread_; /**< The work thread. */
    std::condition_variable cv_; /**< A condition variable to signal the work thread to wake up. */
    Timer timer_; /**< A timer to control the update interval. */
    std::function<void()> sort_; /**< The update function. */
};

#define ProcessWatcherInstance Singleton<ProcessWatcher>::getInstance()

#endif // _PROCESS_WATCHER_H_