#if !defined(__IPROCESS_READER_H__)
#define __IPROCESS_READER_H__

#include <string>

class IProcessReader
{
public:
    virtual ~IProcessReader() = default;
    virtual std::string read() = 0;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void reopen() = 0;
    virtual bool isOpen() const = 0;
    virtual bool isClosed() const = 0;
    virtual void setSort(char order) = 0;
    virtual double getCpuUsage(int pid) = 0;
};

#endif // __IPROCESS_READER_H__