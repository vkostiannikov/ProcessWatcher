#if !defined(_ISUBSCRIBER_H_)
#define _ISUBSCRIBER_H_

class ISubscriber
{
public:
    virtual ~ISubscriber() = default;
    virtual void update() = 0;
};

#endif // _ISUBSCRIBER_H_