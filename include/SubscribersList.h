#if !defined(_SUBSCRIBERS_LIST_H_)
#define _SUBSCRIBERS_LIST_H_

#include <list>
#include <mutex>

template <typename T>
class SubscribersList
{
public:
    SubscribersList() = default;
    ~SubscribersList() = default;

    void add(T subscriber)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        subscribers_.push_back(subscriber);
    }

    void remove(T subscriber)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        subscribers_.remove(subscriber);
    }

    T getNext()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (subscribers_.empty() || currentSubscriber_ == subscribers_.end())
        {
            return nullptr;
        }

        return *currentSubscriber_++;
    }

    void resetNext()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        currentSubscriber_ = subscribers_.begin();
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        subscribers_.clear();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return subscribers_.empty();
    }

private:
    std::list<T> subscribers_;
    mutable std::mutex mtx_;
    typename std::list<T>::iterator currentSubscriber_;
};

#endif // _SUBSCRIBERS_LIST_H_