#if !defined(_SINGLETON_H_)
#define _SINGLETON_H_

template <typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif // _SINGLETON_H_